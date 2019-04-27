#include "Joystick.h"

#include "GLFW/glfw3.h"

#include <iostream>

Joystick::Joystick() {

}

Joystick::Joystick(int i) {
	/*id = GLFW_JOYSTICK_1;*/

	id = GetId(i);

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

int Joystick::GetId(int i) {
	switch (i) {
	case 0: return GLFW_JOYSTICK_1;
	case 1: return GLFW_JOYSTICK_2;
	case 2: return GLFW_JOYSTICK_3;
	case 3: return GLFW_JOYSTICK_4;
	case 4: return GLFW_JOYSTICK_5;
	case 5: return GLFW_JOYSTICK_6;
	case 6: return GLFW_JOYSTICK_7;
	case 7: return GLFW_JOYSTICK_8;
	case 8: return GLFW_JOYSTICK_9;
	case 9: return GLFW_JOYSTICK_10; 
	case 10: return GLFW_JOYSTICK_11;
	case 11: return GLFW_JOYSTICK_12;
	case 12: return GLFW_JOYSTICK_13;
	case 13: return GLFW_JOYSTICK_14;
	case 14: return GLFW_JOYSTICK_15;
	default: return -1;
	}
}