#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* fileName);

int main() {
	int success;
	char infoLog[512];

	std::cout << "Hello, world!" << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Tutorial", NULL, NULL);
	if (window == NULL) { // window not created
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// SHADERS=======================
	// instruct how GPU should process vertex data

	// vertex
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderSrc = loadShaderSrc("src/vertex_core.glsl");
	const GLchar* vertShader = vertShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
	}

	// fragment core
	unsigned int fragmentShaders[2];

	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderSrc = loadShaderSrc("src/fragment_core.glsl");
	const GLchar* fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[0]);

	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
	}

	/*fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	fragShaderSrc = loadShaderSrc("src/fragment_core_2.glsl");
	fragShader = fragShaderSrc.c_str();
	glShaderSource(fragmentShaders[1], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[1]);

	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
	}*/

	// combine shaders in shader program
	unsigned int shaderPrograms[2];
	shaderPrograms[0] = glCreateProgram();
	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);

	/*shaderPrograms[1] = glCreateProgram();
	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);*/

	glLinkProgram(shaderPrograms[0]);
	//glLinkProgram(shaderPrograms[1]);

	glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
	}

	/*glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM2::LINKING_FAILED" << std::endl << infoLog << std::endl;
	}*/

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	//glDeleteShader(fragmentShaders[1]);

	// LINK VERTEX ATTRIBUTES ====================
	// tell GPU how to interpret vertex data

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};
	//unsigned int indices[] = {
	//	0, 1, 2,	// first triangle
	//	2, 3, 4		// second triangle
	//};

	// vertex buffer object, vertex arrays, element buffer objects
	unsigned int VBOs[2], VAOs[2];
	//unsigned int EBO;
	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	//glGenBuffers(1, &EBO);

	// first triangle
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // vertexAttributeId, vertexAttrSize, type, normalize?, stride(space btwn vertexAttrs), offset
	glEnableVertexAttribArray(0);

	// second triangle
	/*glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);*/

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// use shader program
		glUseProgram(shaderPrograms[0]);
		// first triangle
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// second triangle
		/*glUseProgram(shaderPrograms[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 2, 3);*/

		// draw triangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char* fileName) {
	std::ifstream in_file;
	std::stringstream buffer;

	std::string ret = "";

	in_file.open(fileName);

	if (in_file.is_open()) {
		buffer << in_file.rdbuf();

		ret = buffer.str();
	}
	else {
		std::cout << "ERROR::SHADER::COULD_NOT_OPEN_FILE: " << fileName << std::endl;
	}

	in_file.close();

	return ret;
}