#include "joystick.h"

#include <iostream>

Joystick::Joystick(int i) {
	id = getId(i);

	update();
}

void Joystick::update() {
	present = glfwJoystickPresent(id);

	if (present) {
		name = glfwGetJoystickName(id);
		axes = glfwGetJoystickAxes(id, &axesCount);
		buttons = glfwGetJoystickButtons(id, &buttonCount);
	}
}

float Joystick::triggerState(int trigger) {
	if (present) {
		return axes[trigger];
	}

	return -1;
}

unsigned char Joystick::buttonState(int button) {
	if (present) {
		return buttons[button];
	}

	return GLFW_RELEASE;
}

bool Joystick::isPresent() {
	return present;
}

int Joystick::getId(int i) {
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