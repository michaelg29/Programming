#include "Flapper.h"

#include "../Engine/Engine.h"

Flapper::Flapper() {
	flapForce = 360;
	maxRot = 30;
	minRot = -maxRot;
}

Flapper::Flapper(Sprite _sprite) : Flapper() { 
	sprite = _sprite;
	rb.Initialize(0.8f, -10, sprite.GetPos(), sprite.GetRot(), sprite.GetScale(), sprite.GetSize());
}

void Flapper::Update() {
	sprite.Update();
	rb.Update();

	float yVel = rb.GetVel().y;
	if (flapForce == 0) {
		std::cout << "Error! Flapping will do you no good! Setting to 360" << std::endl;
		flapForce = 360;
	}
	float newRot = (maxRot / flapForce) * yVel;
	sprite.RotateTo(newRot);
}

void Flapper::Render() {
	sprite.Render();
	rb.Render(Vector3(0, 0, 0));
}

void Flapper::Flap() {
	rb.SetVel(Vector3(0, flapForce, 0));
	sprite.RotateTo(maxRot);
}

Sprite* Flapper::GetSprite() {
	return &sprite;
}

Rigidbody* Flapper::GetRigidbody() {
	return &rb;
}