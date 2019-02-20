package com.michael.views;

import com.michael.util.Views;

import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

/**
 * Servlet implementation class MyServlet
 */
@WebServlet("/FirstServlet")
public class FirstServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;
	public static final String HTML_START = "<html><body>";
	public static final String HTML_END = "</body></html";
       
    public FirstServlet() {
        super();
    }

	protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		Views.View(request, response, "html/input.html");
	}

	protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		String name = request.getParameter("name"),
				address = request.getParameter("address"),
				phone = request.getParameter("phone");
		
		request.getSession().setAttribute("name", name);
		request.getSession().setAttribute("address", address);
		request.getSession().setAttribute("phone", phone);
		
		response.sendRedirect("jsp/output.jsp");
	}
}
