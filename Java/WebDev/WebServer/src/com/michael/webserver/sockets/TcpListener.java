package com.michael.webserver.sockets;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Executors;

import com.michael.webserver.util.Logger;

public class TcpListener {
    protected int port                    	= 0000;
    protected String addr_str             	= "";
    protected InetAddress addr            	= null;
    protected ServerSocket ss             	= null;
    protected List<Socket> clients 			= null;
    protected boolean running             	= false;
    protected Logger logger					= null;

    public TcpListener(String addr, int port, Logger logger) {
        this.port = port;
        this.addr_str = addr;
    }

    public void run() {
    	if (!running) {
    		try {
    			this.addr = InetAddress.getByName(addr_str);
    		} catch (UnknownHostException e1) {
    			logger.Log("Could not find address.");
    			return;
    		}
            this.clients = new ArrayList<>();

            try {
                this.ss = new ServerSocket(this.port, -1, this.addr);
            } catch (Exception e) {
            	logger.Log("Failed to start server.");
            	return;
            }

            running = true;
            
            while (running) {
            	try {
            		final Socket client = ss.accept();
            		onClientConnected(client);
            		clients.add(client);
            		
            		Executors.newSingleThreadExecutor().execute(new Runnable() {
            		    @Override
            		    public void run() {
            		        clientHandler(client);
            		    }
            		});
            	} catch (Exception e) {
            		logger.Log(e.getMessage());
            		try {
    					ss.close();
    				} catch (IOException e1) {
    					logger.Log("Could not stop server");
    				}
            	}
            }
            
            try {
    			ss.close();
    		} catch (IOException e1) {
    			logger.Log("Could not stop server");
    		}
    	}
    }
    
    public void send(Socket client, String message) throws IOException {
    	DataOutputStream dos = new DataOutputStream(client.getOutputStream());
    	send(dos, message);
    }
    
    public void send(DataOutputStream dos, String message) throws IOException {
    	dos.writeUTF(message);
    	dos.flush();
    }
    
    public void broadcast(String message) throws IOException {
    	for (Socket client : clients) {
    		send(client, message);
    	}
    }
    
    private void clientHandler(Socket client) {
    	DataInputStream dis = null;
    	DataOutputStream dos = null;
		try {
			dis = new DataInputStream(client.getInputStream());
			dos = new DataOutputStream(client.getOutputStream());
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		String received;

        while (true) {
            try {
                received = dis.readUTF();
                messageReceieved(client, received);

                if (received.equals("Exit")) {
                    client.close();
                    clients.remove(client);
                    break;
                }
            } catch (Exception e) { 
            	clients.remove(client);
                onClientDisconnected(client); 
                break;
            } 
        }

        try { 
            dis.close(); 
            dos.close(); 
              
        } catch(IOException e){ 
            e.printStackTrace(); 
        }
    }
    
    protected void onClientConnected(Socket client) {
    	System.out.println(client + " connected");
    }
    
    protected void onClientDisconnected(Socket client) {
    	System.out.println(client + " disconnected");
    }
    
    protected void messageReceieved(Socket client, String message) {
    	System.out.printf("%s said: %s\n", client.toString(), message);
    	try {
			send(client, message);
		} catch (IOException e) {
			e.printStackTrace();
		}
    }
}