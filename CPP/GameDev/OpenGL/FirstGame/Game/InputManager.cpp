#include "InputManager.h"

InputManager::InputManager(Flapper* _flapper) :
	flapper(_flapper) {

}

void InputManager::Update(Engine &engine) {
	Joystick j = engine.GetJoystick(0);

	if (Keyboard::Key(GLFW_KEY_SPACE) || Mouse::ButtonDown(GLFW_MOUSE_BUTTON_LEFT) || j.ButtonState(GLFW_JOYSTICK_PS_X)) {
		flapper->Flap();
	}

	/*if (Mouse::ButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		flapper->GetSprite()->RotateBy(100);
	if (Mouse::ButtonUp(GLFW_MOUSE_BUTTON_RIGHT))
		flapper->GetSprite()->RotateBy(-100);
	if (Mouse::Button(GLFW_MOUSE_BUTTON_MIDDLE))
		flapper->GetSprite()->RotateBy(100);

	if (Keyboard::Key(GLFW_KEY_W) || j.ButtonState(GLFW_JOYSTICK_PS_UP) == GLFW_PRESS)
		flapper->GetRigidbody()->AddForce(Vector3(0, 20, 0));
	if (Keyboard::Key(GLFW_KEY_S) || j.ButtonState(GLFW_JOYSTICK_PS_DOWN) == GLFW_PRESS)
		flapper->GetRigidbody()->AddForce(Vector3(0, -20, 0));
	if (Keyboard::Key(GLFW_KEY_A) || j.ButtonState(GLFW_JOYSTICK_PS_LEFT) == GLFW_PRESS)
		flapper->GetRigidbody()->AddForce(Vector3(-20, 0, 0));
	if (Keyboard::Key(GLFW_KEY_D) || j.ButtonState(GLFW_JOYSTICK_PS_RIGHT) == GLFW_PRESS)
		flapper->GetRigidbody()->AddForce(Vector3(20, 0, 0));
	if (Keyboard::Key(GLFW_KEY_R) || j.ButtonState(GLFW_JOYSTICK_PS_CIRCLE) == GLFW_PRESS)
		flapper->GetSprite()->MoveTo(Vector3(100, 100, 0));*/
}