#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "io/joystick.h"
#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/screen.h"
#include "io/camera.h"

void processInput(GLFWwindow* window, double deltaTime);

float level = 0.2f;
float offset = 0.0f;

Joystick mainJ(0);
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));
bool Camera::usingPrimaryCamera = true;
Camera Camera::secondaryCamera(glm::vec3(3.0f, 3.0f, 3.0f));

double deltaTime = 0.0f;	// Time between current frame and last frame
double lastFrame = 0.0f;	// Time of last frame

int main() {
	std::cout << "Hello, OpenGL!" << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(Screen::SCR_WIDTH, Screen::SCR_HEIGHT, "OpenGL Tutorial", NULL, NULL);
	if (window == NULL) { // window not created
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // capture cursor in center of screen
	glfwSetCursorPosCallback(window, Mouse::mousePositionCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, Screen::SCR_WIDTH, Screen::SCR_HEIGHT);

	// SHADERS=======================
	// instruct how GPU should process vertex data

	Shader ourShader("assets/vertex_core.glsl", "assets/fragment_core.glsl");

	// LINK VERTEX ATTRIBUTES ====================
	// tell GPU how to interpret vertex data

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	/*float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};*/
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// vertex buffer object, vertex arrays, element buffer objects
	unsigned int VBO, VAO; //, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // vertexAttributeId, vertexAttrSize, type, normalize?, stride(space btwn vertexAttrs), offset
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TEXTURES================================

	// generate texture
	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// image wrap
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// also GL_TEXTURE_WRAP_R for 3d (s, t, r) = (x, y, z)

	// border color
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// image filtering
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // scale down -> color nearest to center
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // scale up -> blend colors nearby
	/*
	mipmaps - diff resolution for diff distances
	GL_NEAREST_MIPMAP_NEAREST - nearest mipmap and nearest neighbor interpolation
	GL_LINEAR_MIPMAP_NEAREST - nearest mipmap level and linear interpolation
	GL_NEAREST_MIPMAP_LINEAR - linear mipmap combination between two nearest and neighbor interpolation
	GL_LINEAR_MIPMAP_LINEAR - linear mipmap combination, linear interpolation
	*/

	// load image
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stbi to flip image
	unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("assets/face.png", &width, &height, &nrChannels, 0);
	if (data) {
		// RGBA because png
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	while (!glfwWindowShouldClose(window)) {
		// delta time
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		processInput(window, deltaTime);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// use shader program
		ourShader.use();

		// update position
		//ourShader.setFloat("offset", offset);
		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime()) / 100, glm::vec3(0.1f, 0.1f, 0.1f));
		//ourShader.setMat4("transform", trans);

		// update mix level
		ourShader.setFloat("mixValue", level);

		/*trans = glm::translate(trans, glm::vec3(0.5f, 0.5f, 0.0f));
		ourShader.setMat4("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		trans = glm::translate(trans, glm::vec3(-0.5f, -0.5f, 0.0f));
		ourShader.setMat4("transform", trans);*/

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(x, y, z)); // move camera back = move scene forward in Right Hand Coordinate System
		projection = glm::perspective(glm::radians(Camera::usingPrimaryCamera ? Camera::defaultCamera.zoom : Camera::secondaryCamera.zoom), (float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f); // field of view, aspect ratio, near view, far view

		//const float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//cameraPos = glm::vec3(camX, 0.0f, camZ);
		view = Camera::usingPrimaryCamera ? Camera::defaultCamera.getViewMatrix() : Camera::secondaryCamera.getViewMatrix();
		
		ourShader.setMat4("view", view);
		ourShader.setMat4("projection", projection); // can set outside of loop

		glBindVertexArray(VAO);
		// draw triangle
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

bool tabDownAlready = false;
void processInput(GLFWwindow* window, double deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	if (Keyboard::key(GLFW_KEY_UP)) {
		level += 0.001f;
		if (level >= 1.0f) {
			level = 1.0f;
		}
	}
	else if (Keyboard::key(GLFW_KEY_DOWN)) {
		level -= 0.001f;
		if (level <= 0.0f) {
			level = 0.0f;
		}
	}

	//mainJ.update();

	// update camera
	if (Keyboard::keyWentDown(GLFW_KEY_TAB)) {
		Camera::usingPrimaryCamera = !Camera::usingPrimaryCamera;
	}

	// camera position
	CameraMovement direction = CameraMovement::NONE;

	if (Keyboard::key(GLFW_KEY_W)) {
		direction = CameraMovement::FORWARD;
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		direction = CameraMovement::LEFT;
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		direction = CameraMovement::BACKWARD;
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		direction = CameraMovement::RIGHT;
	}
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		direction = CameraMovement::UP;
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		direction = CameraMovement::DOWN;
	}

	if (direction != CameraMovement::NONE) {
		if (Camera::usingPrimaryCamera) {
			Camera::defaultCamera.updateCameraPos(direction, deltaTime);
		}
		else {
			Camera::secondaryCamera.updateCameraPos(direction, deltaTime);
		}
	}
}