package com.michael.util.views;

import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.michael.util.Views;

/**
 * Servlet implementation class ReturnToForm
 */
public class ReturnToForm extends HttpServlet {
	private static final long serialVersionUID = 1L;
	private String path;
       
    public ReturnToForm(String path) {
        super();
        this.path = path;
    }

	protected final void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		getHandler(request, response);
		request.getSession().setAttribute("next", request.getHeader("referer"));
		Views.View(request, response, path);
	}

	protected final void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
		postHandler(request, response);
		
		if (request.getSession().getAttribute("next") == null)
			response.sendRedirect(request.getContextPath());
		else
			response.sendRedirect(request.getSession().getAttribute("next").toString());
		
		request.getSession().setAttribute("next", null);
	}
	
	protected void getHandler(HttpServletRequest request, HttpServletResponse response) {
		
	}

	protected void postHandler(HttpServletRequest request, HttpServletResponse response) {
		
	}
}
