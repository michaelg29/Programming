#include "Engine/Engine.h"
#include "Engine/Graphics/Sprite.h"
#include "Engine/Math/Vector3.h"

#include "Game/InputManager.h"
#include "Game/Flapper.h"

#include <iostream>

int main() {
	std::cout << "Hello, World!" << std::endl;
	
	Engine engine;
	
	engine.Initialize("First Game");
	engine.UseJoysticks();
	
	Sprite testSprite = Sprite("Assets/Art/Biplane.png", Vector3(Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2, 0));
	testSprite.SetScale(0.25f);

	Flapper player(testSprite);

	InputManager im(&player);

	while (true) {
		engine.Update();
		player.Update();
		im.Update(engine);

		engine.BeginRender();
		player.Render();
		engine.EndRender();
	}

	return 0;
}