package com.test.webserver;

import com.michael.webserver.util.Logger;

public class WebServerLogger implements Logger {
	@Override
	public void Log(String message) {
		System.out.println("Server says: " + message);
	}
}
