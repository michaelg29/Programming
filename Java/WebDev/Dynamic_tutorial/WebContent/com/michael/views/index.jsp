<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
	<meta charset="ISO-8859-1">
	<title>Results</title>
</head>
<body>
	<a href="/Dynamic_tutorial/FirstServlet">Click here</a><br/>
	<%= session.getAttribute("path") %>
</body>
</html>