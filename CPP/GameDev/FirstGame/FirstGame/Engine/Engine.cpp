#include "Engine.h"
#include "IO/Mouse.h"
#include "IO/Keyboard.h"

#include <iostream>

int Engine::SCREEN_WIDTH = 1024;
int Engine::SCREEN_HEIGHT = 768;
GLFWwindow* Engine::window = NULL;
double Engine::dt = 0;

double Engine::GetDT() {
	return dt;
}

Engine::Engine() {
	
}

Engine::~Engine() {

}

bool Engine::Initialize(const char* windowTitle) {
	if (!glfwInit()) {
		std::cout << "error initializing glfw" << std::endl;
		return false;
	}

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle, NULL, NULL);

	if (window == NULL) {
		std::cout << "Error creating window" << std::endl;
		return false;
	}

	// GLFW setup
	glfwMakeContextCurrent(window);
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapInterval(1);

	glfwSetCursorPosCallback(window, Mouse::MousePosCallback);
	glfwSetMouseButtonCallback(window, Mouse::MouseButtonCallback);

	glfwSetKeyCallback(window, Keyboard::KeyCallback);

	for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		joysticks[i] = Joystick(i);
	}

	//joysticks[0] = Joystick(GLFW_JOYSTICK_1);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int xPos = (mode->width - SCREEN_WIDTH) / 2;
	int yPos = (mode->height - SCREEN_HEIGHT) / 2;
	glfwSetWindowPos(window, xPos, yPos);

	// GL setup
	// viewport
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -10, 10);
	glDepthRange(-10, 10);
	glMatrixMode(GL_MODELVIEW);

	// alpha blending
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	lastTime = glfwGetTime();
	
	return true;
}

void Engine::Update() {
	double now = glfwGetTime();
	dt = (now - lastTime);
	lastTime = now;

	for (Joystick &j : joysticks) {
		j.Update();
	}

	glfwPollEvents();
}

void Engine::BeginRender() {
	// clear
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::EndRender() {
	// draw (in main) all sprites
	
	// swap
	glfwSwapBuffers(window);
}

Joystick Engine::GetJoystick(int id) {
	return joysticks[id];
}