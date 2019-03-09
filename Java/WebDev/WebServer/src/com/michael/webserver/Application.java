package com.michael.webserver;

import java.io.IOException;
import java.util.Map;

import com.michael.webserver.requests.View;
import com.michael.webserver.sockets.WebServer;
import com.michael.webserver.util.Logger;

public class Application {
	private static WebServer instance;
	
	public static void instantiate(String ip, int port, Logger logger) throws IOException {
		instance = new WebServer(port, ip, logger);
	}
	
	public static void run() throws IOException {
		instance.run();
	}
	
	String contextRoot;
	String templateRoot;
	String styleRoot;
	String defaultFile;
	String errorFile;
	Map<String, View> views;

	public static void setContextRoot(String contextRoot) {
		instance.setContextRoot(contextRoot);
	}

	public static void setTemplateRoot(String templateRoot) {
		instance.setTemplateRoot(templateRoot);
	}

	public static void setStyleRoot(String styleRoot) {
		instance.setStyleRoot(styleRoot);
	}

	public static void setDefaultFile(String defaultFile) {
		instance.setDefaultFile(defaultFile);
	}

	public static void setErrorFile(String errorFile) {
		instance.setErrorFile(errorFile);
	}
	
	public void addView(String route, View view) {
		instance.addView(route, view);
	}
}
