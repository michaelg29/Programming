#include "mouse.h"
#include "screen.h"
#include "camera.h"

double Mouse::x = Screen::SCR_WIDTH / 2;
double Mouse::y = Screen::SCR_HEIGHT / 2;

double Mouse::lastX = Screen::SCR_WIDTH / 2;
double Mouse::lastY = Screen::SCR_HEIGHT / 2;

double Mouse::dx = 0;
double Mouse::dy = 0;

bool Mouse::firstMouse = true;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::mousePositionCallback(GLFWwindow* window, double _x, double _y) {
	x = _x;
	y = _y;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	dx = x - lastX;
	dy = lastY - y; // y coordinates reversed
	lastX = x;
	lastY = y;

	Camera::defaultCamera.updateCameraDirection(dx, dy);
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	buttons[button] = action != GLFW_RELEASE;
}

void Mouse::mouseWheelCallback(GLFWwindow* window, double dx, double dy) {
	Camera::defaultCamera.updateCameraZoom(dy);
}

double Mouse::getMouseX() {
	return x;
}

double Mouse::getMouseY() {
	return y;
}

double Mouse::getDX() {
	return dx;
}

double Mouse::getDY() {
	return dy;
}

bool Mouse::button(int _button) {
	return buttons[_button];
}