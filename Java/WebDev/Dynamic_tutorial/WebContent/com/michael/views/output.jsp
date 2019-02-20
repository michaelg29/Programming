<%@ page language="java" contentType="text/html; charset=ISO-8859-1"
    pageEncoding="ISO-8859-1"%>
<!DOCTYPE html>
<html>
<head>
	<meta charset="ISO-8859-1">
	<title>Results</title>
</head>
<body>
	<ul>
		<li><%= session.getAttribute("name") %></li>
		<li><%= session.getAttribute("address") %></li>
		<li><%= session.getAttribute("phone") %></li>
	</ul>
</body>
</html>