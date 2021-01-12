import json
import os
import sys
import random
import matplotlib
import threading
import time
matplotlib.use('Qt5Agg')

from PyQt5 import QtCore, QtWidgets
import PyQt5

from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure


def follow_file(fp):
    '''
    Generator function that yields new lines in a file
    '''
    # Seek the end of the file
    #fp.seek(0, os.SEEK_END)
    
    # Start infinite loop
    while True:
        # read last line of file
        line = fp.readline()        # Sleep if file hasn't been updated
        if not line:
            time.sleep(0.1)
            continue

        yield line


class RollingGraphPlot(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100, x_source=None):
        fig = Figure(figsize=(width, height), dpi=dpi)
        super(RollingGraphPlot, self).__init__(fig)
        self.x_source = x_source
        n_data = 600
        self.axes = fig.add_subplot(111)
        self.xdata = list(range(n_data))
        self.ydata = [100,-100] + [0] * (n_data - 2)
        self._plot_ref = self.axes.plot(self.xdata, self.ydata, 'r')[0]

    def update_plot(self):
        # Drop off the first y element, append a new one
        self.ydata = self.ydata[1:] + [self.x_source.value]
        self._plot_ref.set_ydata(self.ydata)
        # Trigger the canvas to update and redraw
        self.draw()


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        self.newest_tg_data = kwargs['tg_data']
        del kwargs['tg_data']
        super(MainWindow, self).__init__(*args, **kwargs)

        self.raw_data_plot = RollingGraphPlot(self, width=6, height=4, dpi=100, x_source=self.newest_tg_data['raw_value'])  
        self.attention_plot = RollingGraphPlot(self, width=4, height=4, dpi=100, x_source=self.newest_tg_data['eSense']['attention'])
        self.meditation_plot = RollingGraphPlot(self, width=4, height=4, dpi=100, x_source=self.newest_tg_data['eSense']['meditation'])

        layout = QtWidgets.QHBoxLayout()
        central_widget = PyQt5.QtWidgets.QWidget(self)
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

        layout.addWidget(self.raw_data_plot)
        layout.addWidget(self.attention_plot)
        layout.addWidget(self.meditation_plot)

        self.show()

        # Setup a timer to trigger the redraw by calling update_plot
        self.timer = QtCore.QTimer()
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.update_graphs)
        self.timer.start()

    def update_graphs(self):
        self.raw_data_plot.update_plot()
        self.attention_plot.update_plot()
        self.meditation_plot.update_plot()


class AtomicInteger():
    def __init__(self, value=0):
        self._value = int(value)
        self._lock = threading.Lock()

    @property
    def value(self):
        with self._lock:
            return self._value

    @value.setter
    def value(self, v):
        with self._lock:
            self._value = int(v)
            return self._value


def read_values_from_file(tg_data):
    for j in follow_file(sys.stdin):
        try:
            json_obj = json.loads(j)
            if 'rawEeg' in json_obj:
                tg_data['raw_value'].value = json_obj['rawEeg']
            if 'eSense' in json_obj:
                tg_data['eSense']['attention'].value = json_obj['eSense']['attention']
                tg_data['eSense']['meditation'].value = json_obj['eSense']['meditation']
        except Exception as e:
            print(e)


def main():
    tg_data = {
        'raw_value': AtomicInteger(), 
        'eSense': {
            'meditation': AtomicInteger(),
            'attention': AtomicInteger()
        }
    }
    t = threading.Thread(target=read_values_from_file, args=(tg_data,))
    t.start()
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow(tg_data=tg_data)
    app.exec_()


if __name__ == "__main__":
    main()
