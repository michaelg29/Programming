#ifndef SPRITE
#define SPRITE

#include "GLFW/glfw3.h"
#include "Texture.h"

#include <string>
#include <iostream>
#include "../Math/Vector3.h"

class Sprite {
public:
	Sprite();
	Sprite(std::string imagePath);
	Sprite(std::string imagePath, Vector3 _pos);

	void Update();
	void Render();

	void SpeedTo(float x);
	void SpeedBy(float x);

	void MoveTo(Vector3 v); // absolutely
	void MoveBy(Vector3 v); // relatively

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void RotateTo(float x);	// absolutely
	void RotateBy(float x);	// relatively

	void SetScale(float x);
	void SetScale(Vector3 v);

	Vector3* GetPos();
	float* GetRot();
	Vector3* GetScale();
	Vector3* GetSize();

private:
	Texture texture;

	float speed;

	Vector3 pos;
	float rot;
	Vector3 scale;

	Vector3 size;
};

#endif