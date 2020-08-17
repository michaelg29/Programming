package com.networking.sockets;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import com.networking.util.Logger;

public class TcpListener {
	protected int port;
	protected String ipAddr_str;
	protected InetAddress ipAddr;
	protected Logger logger;
	protected ServerSocket socket;
	protected boolean running;
	protected boolean bound;
	protected List<TcpClient> clients;
	
	private Scanner scanner;
	
	public TcpListener(int port, String ipAddr_str, Logger logger)  {
		this.port = port;
		this.ipAddr_str = ipAddr_str;
		this.logger = logger;
		this.running = false;
		this.bound = false;
	}
	
	public final void bind() throws IOException {
		this.ipAddr = InetAddress.getByName(ipAddr_str);
		this.socket = new ServerSocket(this.port, -1, this.ipAddr);
		clients = new ArrayList<>();
		this.running = false;
		this.bound = true;
		
		logger.Log("Server bound");
	}
	
	public void run() throws IOException {
		if (!running && bound) {
			this.running = true;
			
			logger.Log("Server started");
			
			Runnable cmdRunnable = () -> {
				commandThread();
			};
			Thread cmdThread = new Thread(cmdRunnable);
			cmdThread.start();
			
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
			
			for (TcpClient client : clients) {
				if (client.socket.isConnected()) {
					client.socket.close();
				}
			}
			
			socket.close();
			
			running = false;
			bound = false;
			
			logger.Log("Server stopped");
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
	
	public final TcpClient getClient(Socket sock) {
		for (TcpClient client : clients) {
			if (client.socket == sock) {
				return client;
			}
		}
		
		return null;
	}
	
	public void stopServer() throws IOException {
		this.running = false;
		// start new client and connect to get to next iteration of main loop
		Socket socket = new Socket(ipAddr_str, port);
		socket.close();
	}
	
	private void clientThread(TcpClient client) {
		while (this.running && client.socket.isConnected()) {
			try {
				String input = client.reader.readLine();
				if (input == null) {
					clients.remove(client);
					onClientDisconnected(client.socket);
					break;
				}
				client.send("Msg received");
				onMessageReceived(client.socket, input);
			} catch (Exception e) {
				e.printStackTrace();
				clients.remove(client);
				onClientDisconnected(client.socket);
				break;
			}
		}
	}
	
	private void commandThread() {
		scanner = new Scanner(System.in);
		
		while (this.running) {
			String cmd = scanner.nextLine();
			
			try {
				switch (cmd) {
				case "stop":
					this.running = false;
					// start new client and connect to get to next iteration of main loop
					Socket socket = new Socket(ipAddr_str, port);
					socket.close();
					break;
				default:
					broadcast(cmd);
					break;
				}
			} catch (Exception e) {
				e.printStackTrace();
				return;
			}
		}
	}
	
	protected void onClientConnected(Socket client) {
		logger.Log("client connected");
	}
	
	protected void onClientDisconnected(Socket client) {
		logger.Log("client disconnected");
	}
	
	protected void onMessageReceived(Socket sender, String msg) {
		logger.Log("msg received: " + msg);
	}
}