#ifndef INPUT_MANAGER
#define INPUT_MANAGER

#include "Flapper.h"
#include "../Engine/Engine.h"
#include "../Engine/IO/Mouse.h"
#include "../Engine/IO/Keyboard.h"
#include "../Engine/IO/Joystick.h"

class InputManager {
public:
	InputManager(Flapper* _flapper);

	void Update(Engine &engine);

private:
	Flapper* flapper;
};

#endif