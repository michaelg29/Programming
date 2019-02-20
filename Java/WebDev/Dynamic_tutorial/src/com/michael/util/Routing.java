package com.michael.util;

import javax.servlet.http.HttpServletRequest;

public class Routing {
	public static void setToReturnAfterForm(HttpServletRequest request) {
		request.getSession().setAttribute("next", request.getHeader("referer"));
	}
}
