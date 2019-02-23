#include "Engine/Engine.h"
#include "Engine/Graphics/Sprite.h"

#include <iostream>

int main() {
	std::cout << "Hello, World!" << std::endl;
	
	Engine engine;
	
	engine.Initialize("First Game");
	
	Sprite testSprite = Sprite("Assets/Art/Biplane.png", 100, 100);

	while (true) {
		engine.Update();
		testSprite.Update();

		engine.BeginRender();
		// render all sprites
		testSprite.Render();
		engine.EndRender();
	}

	return 0;
}