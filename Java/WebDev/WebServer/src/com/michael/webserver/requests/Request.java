package com.michael.webserver.requests;

import com.michael.webserver.sockets.TcpClient;

public class Request {
	String body;
	String response;
	
	public Request(String body) {
		this.body = body;
	}
	
	public void parse() {
		
	}
	
	public void readFile(String relativePath) {
		
	}
	
	public void forward(TcpClient client) {
		client.send(response);
	}
}
