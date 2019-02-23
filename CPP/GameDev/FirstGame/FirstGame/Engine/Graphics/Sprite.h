#ifndef SPRITE
#define SPRITE

#include "GLFW/glfw3.h"
#include "Texture.h"

#include <string>
#include <iostream>

class Sprite {
public:
	Sprite();
	Sprite(std::string imagePath);
	Sprite(std::string imagePath, float _xPos, float _yPos);

	void Update();
	void Render();

	void SpeedTo(float x);
	void SpeedBy(float x);

	void MoveTo(float x, float y); // absolutely
	void MoveBy(float x, float y); // relatively

	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();

	void RotateTo(float x);	// absolutely
	void RotateBy(float x);	// relatively

	void SetScale(float x);
	void SetScale(float x, float y);

private:
	Texture texture;

	float speed;

	float xPos;
	float yPos;
	float rot;
	float xScale;
	float yScale;
};

#endif