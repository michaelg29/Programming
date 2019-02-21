<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<%@ page import="com.michael.models.User" %>
<!DOCTYPE html>
<html>
<head>
	<meta charset="ISO-8859-1">
	<title>Results</title>
</head>
<body>
	<a href="/Dynamic_tutorial/FirstServlet">Click here</a><br/>
	
	<% if (session.getAttribute("user") == null) {%>
		<p>Not logged in</p>
	<% } else { %>
		<p>Logged in</p>
		<p>Hello <%= ((User)session.getAttribute("user")).getFirstname() %></p>
	<% } %>	
</body>
</html>