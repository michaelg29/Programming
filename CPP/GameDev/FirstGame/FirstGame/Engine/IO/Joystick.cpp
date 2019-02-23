#include "Joystick.h"

#include "GLFW/glfw3.h"

#include <iostream>

Joystick::Joystick() {

}

Joystick::Joystick(int i) {
	/*id = GLFW_JOYSTICK_1;*/

	switch (i) {
	case 0: id = GLFW_JOYSTICK_1; break;
	case 1: id = GLFW_JOYSTICK_2; break;
	case 2: id = GLFW_JOYSTICK_3; break;
	case 3: id = GLFW_JOYSTICK_4; break;
	case 4: id = GLFW_JOYSTICK_5; break;
	case 5: id = GLFW_JOYSTICK_6; break;
	case 6: id = GLFW_JOYSTICK_7; break;
	case 7: id = GLFW_JOYSTICK_8; break;
	case 8: id = GLFW_JOYSTICK_9; break;
	case 9: id = GLFW_JOYSTICK_10; break;
	case 10: id = GLFW_JOYSTICK_11; break;
	case 11: id = GLFW_JOYSTICK_12; break;
	case 12: id = GLFW_JOYSTICK_13; break;
	case 13: id = GLFW_JOYSTICK_14; break;
	case 14: id = GLFW_JOYSTICK_15; break;
	}

	present = glfwJoystickPresent(id);

	if (present == 1) {
		name = glfwGetJoystickName(id);
		axes = glfwGetJoystickAxes(id, &axesCount);
		buttons = glfwGetJoystickButtons(id, &buttonCount);
	}
}

void Joystick::Update() {
	present = glfwJoystickPresent(id);

	if (present == 1) {
		name = glfwGetJoystickName(id);
		axes = glfwGetJoystickAxes(id, &axesCount);
		buttons = glfwGetJoystickButtons(id, &buttonCount);
	}
	
}

float Joystick::TriggerState(int trigger) {
	if (present == 1) {
		return axes[trigger];
	}

	return -1;
}

unsigned char Joystick::ButtonState(int button) {
	if (present == 1) {
		return buttons[button];
	}

	return GLFW_RELEASE;
}