package com.michael.webserver.sockets;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import com.michael.webserver.util.Logger;

public class TcpListener {
	protected int port;
	protected String ipAddr_str;
	protected InetAddress ipAddr;
	protected Logger logger;
	protected ServerSocket socket;
	protected boolean running;
	protected List<TcpClient> clients;
	
	public TcpListener(int port, String ipAddr_str, Logger logger)  {
		this.port = port;
		this.ipAddr_str = ipAddr_str;
		this.logger = logger;
		this.running = false;
	}
	
	public void bind() throws IOException {
		this.ipAddr = InetAddress.getByName(ipAddr_str);
		this.socket = new ServerSocket(this.port, -1, this.ipAddr);
		clients = new ArrayList<>();
		this.running = false;
	}
	
	public void run() throws IOException {
		if (!running) {
			this.running = true;
			
			while (this.running) {
				Socket client = socket.accept();
				TcpClient tcpClient = new TcpClient(client);
				clients.add(tcpClient);
				Runnable clientRunnable = () -> {
					clientThread(tcpClient);
				};
				Thread clientThread = new Thread(clientRunnable);
				
				clientThread.start();
				onClientConnected(client);
			}
		}
	}
	
	public final void send(TcpClient receiver, String msg) throws IOException {
		receiver.send(msg);
	}
	
	public final void broadcast(String msg) throws IOException {
		for (TcpClient client : clients) {
			send(client, msg);
		}
	}
	
	private void clientThread(TcpClient client) {
		while (this.running) {
			try {
				String input = client.reader.readLine();
				if (input == null) {
					clients.remove(client);
					onClientDisconnected(client.socket);
					break;
				}
				onMessageReceived(client.socket, input);
				
			} catch (Exception e) {
				e.printStackTrace();
				clients.remove(client);
				onClientDisconnected(client.socket);
				break;
			}
		}
	}
	
	protected void onClientConnected(Socket client) {
		logger.Log(client + " connected");
	}
	
	protected void onClientDisconnected(Socket client) {
		logger.Log(client + " disconnected");
	}
	
	protected void onMessageReceived(Socket sender, String msg) {
		logger.Log(sender + " says: " + msg);
		try {
			broadcast(msg);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}