import java.io.*;
import java.text.*;
import java.util.*;
import java.net.*;

public class TcpListener {
    protected int port                    = 0000;
    protected String addr_str             = "";
    protected InetAddress addr            = null;
    protected ServerSocket ss             = null;
    protected List<ClientHandler> clients = null;
    protected boolean running             = false;

    public TcpListener(int port, String addr) {
        this.port = port;
        this.addr_str = addr;
    }

    public void run() {
        this.addr = InetAddress.getByName(addr_str);
        this.clients = new ArrayList<ClientHandler>();

        try {
            this.ss = new ServerSocket(this.port, -1, this.addr);
        } catch (Exception e) {
            System.out.println("Failed to start server");
        }

        running = true;
    }

    public static void main(String[] args) throws IOException {
        ServerSocket ss = new ServerSocket(9000);

        while (true) {
            Socket s = null;

            try {
                s = ss.accept();

                System.out.println("New client connected: " + s);

                DataInputStream  dis = new DataInputStream(s.getInputStream());
                DataOutputStream dos = new DataOutputStream(s.getOutputStream());

                System.out.println("New thread for client");

                Thread t = new ClientHandler(s, dis, dos);

                t.start();
            } catch (Exception e) {
                s.close();
                e.printStackTrace();
            }
        }
    }
}

class ClientHandler extends Thread {
    final DataInputStream   dis;
    final DataOutputStream  dos;
    final Socket            s;

    public ClientHandler(Socket s, DataInputStream dis, DataOutputStream dos) {
        this.s = s;
        this.dis = dis;
        this.dos = dos;
    }

    @Override
    public void run() {
        String received;
        String toReturn;

        while (true) {
            try {
                dos.writeUTF("Hello, what is your name?");

                received = dis.readUTF();

                if (received.equals("Exit")) {
                    System.out.println("Client " + this.s + " closing conn.");
                    this.s.close();
                    break;
                }

                System.out.println("Client " + this.s + " says: " + received);
            } catch (IOException e) { 
                e.printStackTrace(); 
            } 
        }

        try { 
            this.dis.close(); 
            this.dos.close(); 
              
        } catch(IOException e){ 
            e.printStackTrace(); 
        }
    }
}