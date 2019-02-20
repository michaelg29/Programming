package com.michael.util;

import java.io.IOException;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public class Views {
	public static void View(HttpServletRequest request, HttpServletResponse response, String relative_path) throws ServletException, IOException {
		RequestDispatcher view = request.getRequestDispatcher(relative_path);
		view.forward(request, response);
	}
}
