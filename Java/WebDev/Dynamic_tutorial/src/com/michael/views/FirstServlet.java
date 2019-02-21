package com.michael.views;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.michael.util.views.forms.ReturnToForm;

/**
 * Servlet implementation class MyServlet
 */
@WebServlet("/FirstServlet")
public class FirstServlet extends ReturnToForm {
	private static final long serialVersionUID = 1L;
	public static final String HTML_START = "<html><body>";
	public static final String HTML_END = "</body></html";
       
    public FirstServlet() {
        super("com/michael/views/input.html");
    }
	
	protected void postHandler(HttpServletRequest request, HttpServletResponse response) {
		String name = request.getParameter("name"),
				address = request.getParameter("address"),
				phone = request.getParameter("phone");
		
		request.getSession().setAttribute("name", name);
		request.getSession().setAttribute("address", address);
		request.getSession().setAttribute("phone", phone);
	}
}
