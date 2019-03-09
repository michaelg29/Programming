package com.michael.webserver.requests;

import com.michael.webserver.sockets.TcpClient;

public class View {
	public void respond(TcpClient sender, Request request) {
		request.forward(sender);
	}
}
