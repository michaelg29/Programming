package com.test.webserver;

import com.michael.webserver.sockets.TcpListener;

public class Main {
	public static void main(String[] args) {
		WebServerLogger logger = new WebServerLogger();
		TcpListener tcp = new TcpListener("localhost", 9000, logger);
		tcp.run();
	}
}
