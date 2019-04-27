#ifndef MOUSE
#define MOUSE

#include "GLFW/glfw3.h"

class Mouse {
public:
	static void MousePosCallback(GLFWwindow* window, double _x, double _y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static double GetMouseX();
	static double GetMouseY();

	static bool ButtonDown(int button);	// if pressed
	static bool ButtonUp(int button);	// if put up
	static bool Button(int button);		// get status; down = true; up = false;

private:
	static double x;
	static double y;

	static bool buttons[];
	static bool buttonsDown[];
	static bool buttonsUp[];
};

#endif