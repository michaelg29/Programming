#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/screen.h"
#include "io/camera.h"

#include "graphics/shader.h"

#include "graphics/models/cube.hpp"
#include "graphics/models/textured_cube.hpp"
#include "graphics/models/lamp.hpp"

void processInput(double deltaTime);

float mixVal = 0.5f;

Screen screen;

Joystick mainJ(0);
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 3.0f));

double deltaTime = 0.0f; // tme btwn frames
double lastFrame = 0.0f; // time of last frame

int main() {
	std::cout << "Hello, OpenGL!" << std::endl;

	glfwInit();

	// openGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

# ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COPMPAT, GL_TRUE);
#endif

	if (!screen.init()) {
		std::cout << "Could not open window." << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();

	// shader
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lightShader("assets/object.vs", "assets/lamp.fs");

	// textures
	Texture tex1 = Texture("assets/image1.jpg", "texture0");
	tex1.load();
	Texture tex2 = Texture("assets/image2.png", "texture1");
	tex2.load();

	// objects
	TexturedCube c({ tex1, tex2 }, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	c.init();

	glm::vec3 lampColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 lampPos = glm::vec3(-1.0f, 0.5f, 0.1f);
	Lamp lamp(lampColor, lampPos, glm::vec3(0.25f));
	lamp.init();

	mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present." << std::endl;
	}

	while (!screen.shouldClose()) {
		// calculate dt
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;

		// process input
		processInput(deltaTime);

		// render
		screen.update();

		shader.activate();
		
		shader.setFloat("mixVal", mixVal);
		shader.set3Float("lightColor", lampColor);
		shader.set3Float("lightPos", lampPos);
		shader.set3Float("viewPos", Camera::defaultCamera.cameraPos);
		
		// camera view/projection
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = Camera::defaultCamera.getViewMatrix();
		projection = glm::perspective(
			glm::radians(Camera::defaultCamera.zoom),
			(float)Screen::SCR_WIDTH / (float)Screen::SCR_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		c.render(shader);

		lightShader.activate();
		lightShader.set3Float("lightColor", lampColor);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		lamp.render(lightShader);

		screen.newFrame();
		glfwPollEvents();
	}

	c.cleanup();

	glfwTerminate();
	return 0;
}

void processInput(double deltaTime) {
	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		screen.setShouldClose(true);
	}

	// change mix value
	if (Keyboard::key(GLFW_KEY_UP)) {
		mixVal += .01f;
		if (mixVal > 1) {
			mixVal = 1.0f;
		}
	}
	if (Keyboard::key(GLFW_KEY_DOWN)) {
		mixVal -= .01f;
		if (mixVal < 0) {
			mixVal = 0.0f;
		}
	}

	// move camera
	if (Keyboard::key(GLFW_KEY_W)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::FORWARD, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_S)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::BACKWARD, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_D)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::RIGHT, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_A)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::LEFT, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_SPACE)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::UP, deltaTime);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
		Camera::defaultCamera.updateCameraPos(CameraDirection::DOWN, deltaTime);
	}
}