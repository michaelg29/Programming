package com.michael.util.data.db.sqlserver;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;

public class DbObject {
	Map<String, Object> attributes;
	
	public DbObject() {
		attributes = new HashMap<>();
	}
	
	public DbObject(ResultSet data) throws SQLException {
		attributes = new HashMap<>();
		read(data);
	}
	
	public final void read(ResultSet data) throws SQLException {
		while (data.next()) {
			ResultSetMetaData rsmd = data.getMetaData();
			for (int i = 1; i < rsmd.getColumnCount(); i++) {
				String name = rsmd.getColumnName(i);
				Object value = data.getObject(name);
				attributes.put(name, value);
				handleAttribute(name, value);
			}
		}
	}
	
	public void handleAttribute(String name, Object value) {
		
	}
}
