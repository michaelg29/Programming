#include "libs.h"

// https://www.youtube.com/playlist?list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z part 9

void framebuffer_resize_callback(GLFWwindow* widnow, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

int main() {
	// initialize GLFW
	glfwInit();

	// create window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); Mac os
	
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tutorial", NULL, NULL);

	// For static window size: glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	// For dynamic window size:
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

	glfwMakeContextCurrent(window); // Important

	// init glew (Needs window and OPENGL context)
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	// main loop
	bool gameRunning = true;

	while (!glfwWindowShouldClose(window)) {
		// update input
		glfwPollEvents(); // takes all events in queue

		// glfwSetWindowShouldClose(window, true);

		// UPDATE ---

		// DRAW ---
		// clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// draw

		// end draw
		glfwSwapBuffers(window);
		glFlush();
	}

	// end of program
	glfwTerminate();

	system("pause");
	return 0;
}