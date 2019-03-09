package com.test.webserver;

import java.io.IOException;

import com.michael.webserver.Application;

public class Main {
	public static void main(String[] args) throws IOException {
		WebServerLogger logger = new WebServerLogger();
		Application.instantiate("localhost", 8080, logger);
	}
}
