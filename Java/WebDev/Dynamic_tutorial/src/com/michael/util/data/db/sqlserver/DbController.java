package com.michael.util.data.db.sqlserver;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class DbController {
	private String server, dbName;
	private int port;
	private Connection conn;
	
	public DbController(String server, String dbName, int port) {
		this.server = server;
		this.dbName = dbName;
		this.port = port;
	}
	
	public void login(String username, String password) throws SQLException, ClassNotFoundException {
		Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
        String connectionUrl = "jdbc:sqlserver://" + server + ":" + port + ";" + 
	                                "database=" + dbName + ";" + 
	                                "user=" + username + ";" + 
	                                "password=" + password + ";"; 
        conn = DriverManager.getConnection(connectionUrl); 
	}
	
	public ResultSet executeQuery(String query) throws SQLException {
		Statement stmt = conn.createStatement();
		ResultSet rs = stmt.executeQuery(query);
		return rs;
	}
	
	public void close() throws SQLException {
		conn.close();
	}
}
