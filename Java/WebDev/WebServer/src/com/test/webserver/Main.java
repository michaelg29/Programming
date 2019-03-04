package com.test.webserver;

import java.io.IOException;

import com.michael.webserver.sockets.TcpListener;

public class Main {
	public static void main(String[] args) throws IOException {
		WebServerLogger logger = new WebServerLogger();
		TcpListener tcp = new TcpListener(9000, "localhost", logger);
		tcp.bind();
		tcp.run();
	}
}
