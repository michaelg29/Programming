#include "Engine/Engine.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/IO/Mouse.h"
#include "Engine/IO/Keyboard.h"
#include "Engine/IO/Joystick.h"

#include <iostream>

int main() {
	std::cout << "Hello, World!" << std::endl;
	
	Engine engine;
	
	engine.Initialize("First Game");
	
	Sprite testSprite = Sprite("Assets/Art/Biplane.png", 100, 100);
	testSprite.SetScale(0.25f);

	while (true) {
		engine.Update();
		testSprite.Update();

		//testSprite.SetPos((float)Mouse::GetMouseX(), (float)Mouse::GetMouseY());

		if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_LEFT))
			testSprite.RotateBy(100);
		if (Mouse::ButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
			testSprite.RotateBy(-100);
		if (Mouse::Button(GLFW_MOUSE_BUTTON_MIDDLE))
			testSprite.RotateBy(100);

		for (int i = 0; i < GLFW_JOYSTICK_LAST; i++) {

		}

		if (Keyboard::Key(GLFW_KEY_W) || engine.GetJoystick(GLFW_JOYSTICK_1).ButtonState(GLFW_JOYSTICK_PS_UP) == GLFW_PRESS)
			testSprite.MoveUp();							
		if (Keyboard::Key(GLFW_KEY_S) || engine.GetJoystick(GLFW_JOYSTICK_1).ButtonState(GLFW_JOYSTICK_PS_DOWN) == GLFW_PRESS)
			testSprite.MoveDown();							
		if (Keyboard::Key(GLFW_KEY_A) || engine.GetJoystick(GLFW_JOYSTICK_1).ButtonState(GLFW_JOYSTICK_PS_LEFT) == GLFW_PRESS)
			testSprite.MoveLeft();							
		if (Keyboard::Key(GLFW_KEY_D) || engine.GetJoystick(GLFW_JOYSTICK_1).ButtonState(GLFW_JOYSTICK_PS_RIGHT) == GLFW_PRESS)
			testSprite.MoveRight();							
		if (Keyboard::Key(GLFW_KEY_R) || engine.GetJoystick(GLFW_JOYSTICK_1).ButtonState(GLFW_JOYSTICK_PS_CIRCLE) == GLFW_PRESS)
			testSprite.MoveTo(100, 100);

		// joystick
		//int present = glfwJoystickPresent(GLFW_JOYSTICK_1);
		//if (present == 1) {
		//	int axesCount;
		//	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount); // array
		//	// left stick x axis	- axes[0]
		//	// left stick y axis	- axes[1]
		//	// right stick x axis	- axes[2]
		//	// right stick y axis	- axes[3]
		//	// left trigger			- axes[4]
		//	// right trigger		- axes[5]

		//	int buttonCount;
		//	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		//	for (int i = 0; i < 18; i++) {
		//		if (buttons[i] == GLFW_PRESS) {
		//			std::cout << "Button pressed: " << i << std::endl;
		//		}
		//	}

		//	/*if (buttons[1] == GLFW_PRESS) {
		//		std::cout << "Button pressed" << std::endl << std::endl << std::endl << std::endl;
		//	}
		//	else if (buttons[1] == GLFW_RELEASE) {
		//		std::cout << "Button up" << std::endl;
		//	}*/

		//	const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
		//}

		engine.BeginRender();
		// render all sprites
		testSprite.Render();
		engine.EndRender();
	}

	return 0;
}