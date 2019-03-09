package com.michael.webserver.sockets;

import java.io.IOException;
import java.net.Socket;
import java.util.Map;
import java.util.HashMap;

import com.michael.webserver.util.Logger;
import com.michael.webserver.requests.Request;
import com.michael.webserver.requests.View;

public class WebServer extends TcpListener {
	String contextRoot;
	String templateRoot;
	String styleRoot;
	String defaultFile;
	String errorFile;
	Map<String, View> views;
	
	public WebServer(int port, String ipAddr_str, Logger logger) throws IOException {
		super(port, ipAddr_str, logger);
		
		this.contextRoot = "wwwroot";
		this.templateRoot = "templates";
		this.styleRoot = "styles";
		this.defaultFile = "index.html";
		this.errorFile = "error.html";
		views = new HashMap<>();
		
		bind();
	}
	
	public View getView(String route) {
		if (views.containsKey(route))
			return views.get(route);
		
		return null;
	}
	
	protected void onClientConnected(Socket client) {}
	
	protected void onClientDisconnected(Socket client) {}
	
	protected void onMessageReceived(Socket sender, String msg) {
		Request request = new Request(msg);
		request.parse();

		View view = getView(""); // request.getRoute()
		if (view == null) {
			request.readFile(errorFile);
			request.forward(getClient(sender));
		} else {
			view.respond(getClient(sender), request);
		}
	}

	public String getContextRoot() {
		return contextRoot;
	}

	public void setContextRoot(String contextRoot) {
		this.contextRoot = contextRoot;
	}

	public String getTemplateRoot() {
		return templateRoot;
	}

	public void setTemplateRoot(String templateRoot) {
		this.templateRoot = templateRoot;
	}

	public String getStyleRoot() {
		return styleRoot;
	}

	public void setStyleRoot(String styleRoot) {
		this.styleRoot = styleRoot;
	}

	public String getDefaultFile() {
		return defaultFile;
	}

	public void setDefaultFile(String defaultFile) {
		this.defaultFile = defaultFile;
	}

	public String getErrorFile() {
		return errorFile;
	}

	public void setErrorFile(String errorFile) {
		this.errorFile = errorFile;
	}
	
	public void addView(String route, View view) {
		this.views.put(route, view);
	}
}
