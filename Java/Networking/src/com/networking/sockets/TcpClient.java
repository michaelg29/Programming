package com.networking.sockets;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class TcpClient {
	protected Socket socket;
	protected PrintWriter writer;
	protected BufferedReader reader;
	
	public TcpClient(Socket socket) throws IOException {
		this.socket = socket;
        this.reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.writer = new PrintWriter(socket.getOutputStream(), true);
        this.writer.println("Welcome to the server");
	}
	
	public void send(String msg) {
		this.writer.println(msg);
	}
}
