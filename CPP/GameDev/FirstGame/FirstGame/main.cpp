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
	engine.UseJoysticks();
	
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

		Joystick j = engine.GetJoystick(0);

		if (Keyboard::Key(GLFW_KEY_W) || j.ButtonState(GLFW_JOYSTICK_PS_UP) == GLFW_PRESS)
			testSprite.MoveUp();
		if (Keyboard::Key(GLFW_KEY_S) || j.ButtonState(GLFW_JOYSTICK_PS_DOWN) == GLFW_PRESS)
			testSprite.MoveDown();
		if (Keyboard::Key(GLFW_KEY_A) || j.ButtonState(GLFW_JOYSTICK_PS_LEFT) == GLFW_PRESS)
			testSprite.MoveLeft();
		if (Keyboard::Key(GLFW_KEY_D) || j.ButtonState(GLFW_JOYSTICK_PS_RIGHT) == GLFW_PRESS)
			testSprite.MoveRight();
		if (Keyboard::Key(GLFW_KEY_R) || j.ButtonState(GLFW_JOYSTICK_PS_CIRCLE) == GLFW_PRESS)
			testSprite.MoveTo(100, 100);

		engine.BeginRender();
		// render all sprites
		testSprite.Render();
		engine.EndRender();
	}

	return 0;
}