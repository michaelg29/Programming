package com.michael.models;

import java.sql.ResultSet;
import java.sql.SQLException;

import com.michael.util.data.db.sqlserver.DbObject;

public class User extends DbObject {
	private int id;
	private String username;
	private String email;
	private String firstname;
	private String lastname;
	
	public User(ResultSet data) throws SQLException {
		super(data);
	}
	
	@Override
	public void handleAttribute(String name, Object value) {
		switch (name) {
		case "ID":
			id = Integer.parseInt(value.toString());
			break;
		case "FirstName":
			firstname = value.toString();
			break;
		case "LastName":
			lastname = value.toString();
			break;
		case "Username":
			username = value.toString();
			break;
		case "Email":
			email = value.toString();
			break;
		}
	}

	public int getId() {
		return id;
	}

	public String getUsername() {
		return username;
	}

	public String getEmail() {
		return email;
	}

	public String getFirstname() {
		return firstname;
	}

	public String getLastname() {
		return lastname;
	}
}
