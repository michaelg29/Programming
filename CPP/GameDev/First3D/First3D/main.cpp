#include "libs.h"

// https://www.youtube.com/playlist?list=PL6xSOsbVA1eYSZTKBxnoXYboy7wc4yg-Z part 13

Vertex vertices[] = {
	// Position							// color						// texcoords
	glm::vec3(0.0f, 0.5f, 0.f),			glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),
	glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0, 1, 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* widnow, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program) {
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	// Vertex shader
	in_file.open("vertex_core.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::MAIN.CPP::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << std::endl;
		loadSuccess = false;
	}

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::MAIN.CPP::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	temp = "";
	src = "";

	// Fragment shader
	in_file.open("fragment_core.glsl");

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::MAIN.CPP::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << std::endl;
		loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::MAIN.CPP::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	//Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::MAIN.CPP::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << std::endl;
		std::cout << infoLog << std::endl;
		loadSuccess = false;
	}

	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
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
		std::cout << "ERROR::MAIN.CPP::main::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}

	// opengl options
	glEnable(GL_DEPTH_TEST);					// use z coordinate
	glEnable(GL_CULL_FACE);						// don't draw stuff behind objects
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);						// front face draws vectors going CCW

	glEnable(GL_BLEND);							// enable blending of colors
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// draw using fill, use GL_LINE for outline

	// SHADERS ---
	// shader init
	GLuint core_program;
	if (!loadShaders(core_program)) {
		glfwTerminate();
	}

	// MODEL ---
	// VAO, VBO, EBO ---
	// GEN VAO AND BIND - vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// GEN VBO AND BIND AND SEND DATA - vertex buffer object
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// GEN EBO AND BIND AND SEND DATA - element buffer object
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set vertexattribpointers and enable
	// allow GPU to interpret the vertices, input assembly, locations set in glsl files

	// position ---
	//GLuint attribLoc = glGetAttribLocation(core_program, "vertex_position"); // vertex_core.glsl = 0
	glVertexAttribPointer(0 /* attribLoc */, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// starting index, how many, type, normalized, how many elements until next vertex, amount of memory to next attribute
	glEnableVertexAttribArray(0 /* attribLoc */);

	// color ---
	glVertexAttribPointer(1 /* attribLoc */, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1 /* attribLoc */);

	// texture ---
	glVertexAttribPointer(2 /* attribLoc */, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2 /* attribLoc */);

	// bind VAO 0
	glBindVertexArray(0);

	// main loop
	bool gameRunning = true;

	while (!glfwWindowShouldClose(window)) {
		// update input
		glfwPollEvents(); // takes all events in queue

		// glfwSetWindowShouldClose(window, true);

		// UPDATE ---
		updateInput(window);

		// DRAW ---
		// clear
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// use a program
		glUseProgram(core_program);

		// bind VAO
		glBindVertexArray(VAO);

		// draw
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		// end draw
		glfwSwapBuffers(window);
		glFlush();
	}

	// end of program
	glfwDestroyWindow(window);
	glfwTerminate();

	// delete program
	glDeleteProgram(core_program);

	system("pause");
	return 0;
}