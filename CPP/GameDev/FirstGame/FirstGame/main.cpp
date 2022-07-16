#include "Engine/Engine.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/IO/Mouse.h"
#include "Engine/IO/Keyboard.h"

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

		if (Keyboard::Key(GLFW_KEY_W))
			testSprite.MoveUp();
		if (Keyboard::Key(GLFW_KEY_S))
			testSprite.MoveDown();
		if (Keyboard::Key(GLFW_KEY_A))
			testSprite.MoveLeft();
		if (Keyboard::Key(GLFW_KEY_D))
			testSprite.MoveRight();
		if (Keyboard::Key(GLFW_KEY_R))
			testSprite.MoveTo(100, 100);

		engine.BeginRender();
		// render all sprites
		testSprite.Render();
		engine.EndRender();
	}

	return 0;
}