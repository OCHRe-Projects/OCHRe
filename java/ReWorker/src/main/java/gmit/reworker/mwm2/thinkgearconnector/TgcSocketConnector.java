package gmit.reworker.mwm2.thinkgearconnector;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.CharBuffer;
import java.nio.channels.SocketChannel;
import java.nio.charset.Charset;
import java.nio.charset.CharsetEncoder;
import java.util.Scanner;

public class TgcSocketConnector {
    private static final String DEFAULT_HOST = "127.0.0.1";
    private static final int DEFAULT_PORT = 13854;

    private String host = "";
    private int port = 0;

    private boolean connected = false;

    private SocketChannel channel;
    private Scanner in;

    public TgcSocketConnector()
    {
        this.host = DEFAULT_HOST;
        this.port = DEFAULT_PORT;
    }

    public TgcSocketConnector(String host, int port)
    {
        this.host = host;
        this.port = port;
    }

    public String getHost()
    {
        return host;
    }

    public void setHost(String host)
    {
        this.host = host;
    }

    public int getPort()
    {
        return port;
    }

    public void setPort(int port)
    {
        this.port = port;
    }

    public boolean isConnected()
    {
        return this.connected;
    }

    public void connect() throws IOException
    {
        if (!this.connected) {
            System.out.println("Connecting");
            this.channel = SocketChannel.open(new InetSocketAddress(this.host, this.port));

            CharsetEncoder enc = Charset.forName("UTF-8").newEncoder();
            String jsonCommand = "{\"enableRawOutput\": true, \"format\": \"Json\"}\n";
            this.channel.write(enc.encode(CharBuffer.wrap(jsonCommand)));

            this.in = new Scanner(channel);
            this.connected = true;
        } else {
            System.out.println("Already connected to TGC");
        }
    }

    public boolean isDataAvailable()
    {
        if (this.connected) {
            return this.in.hasNextLine();
        } else {
            return false;
        }
    }

    public String getData()
    {
        return this.in.nextLine();
    }

    public void close() throws IOException
    {

        if (this.connected) {
            System.out.println("closing connection");
            this.in.close();
            this.channel.close();
            this.connected = false;
            System.out.println("connection closed");
        }
    }

}
