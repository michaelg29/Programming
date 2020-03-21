#ifndef MOUSE_H
#define MOUSE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse {
public:
	static void mousePositionCallback(GLFWwindow* window, double _x, double _y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static double getMouseX();
	static double getMouseY();

	static double getDX();
	static double getDY();

	static bool button(int _button);
	static bool buttonChanged(int _button);
	static bool buttonWentUp(int _button);
	static bool buttonWentDown(int _button);

private:
	static double x;
	static double y;

	static double lastX;
	static double lastY;

	static double dx;
	static double dy;

	static bool firstMouse;

	static bool buttons[];
	static bool buttonsChanged[];
};

#endif