package gmit.reworker.mwm2.thinkgearconnector;

import java.io.IOException;

public class TgcSocketConnectorListener implements Runnable {
    private TgcSocketConnector socketConnection;
    private TGEventHandler tgEventHandler;

    private boolean exitRequest = false;

    public TgcSocketConnectorListener() {
        socketConnection = new TgcSocketConnector();
    }

    public TgcSocketConnectorListener(String host, int port) {
        socketConnection = new TgcSocketConnector(host, port);
    }

    public void requestExit() {
        exitRequest = true;
    }

    public void setTGEventHandler(TGEventHandler tgEventHandler) {
        this.tgEventHandler = tgEventHandler;
    }

    @Override
    public void run() {
        try {
            socketConnection.connect();
            System.out.println("Connected");
        }catch(IOException e) {
            System.err.println("Could not establish connection to ThinkGear Connector at " + socketConnection.getHost() + ":" + socketConnection.getPort());
        }
    }
}
