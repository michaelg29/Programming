#include "Sprite.h"
#include "../Engine.h"

Sprite::Sprite() {
	texture = Texture();
	xPos = 0;
	yPos = 0;
	rot = 0;
	speed = 100;
}

Sprite::Sprite(std::string imagePath) :
	Sprite() {
	texture = Texture(imagePath);
}

Sprite::Sprite(std::string imagePath, float _xPos, float _yPos) :
	Sprite() {
	texture = Texture(imagePath);
	xPos = _xPos;
	yPos = _yPos;
}

void Sprite::Update() {
	
}

void Sprite::Render() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.GetID());
	glLoadIdentity();

	// translate -> rotate -> scale
	glTranslatef(xPos, yPos, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(xScale, yScale, 1);

	// rendering
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);

	// take each corner and put it at a scene coordinate; relative to translation
	glTexCoord2f(0, 0);		glVertex2f(0, 0);										// take bottom left and put at (xPos, yPos)
	glTexCoord2f(1, 0);		glVertex2f(texture.GetWidth(), 0);						// take bottom right and put at (xPos, yPos)
	glTexCoord2f(1, 1);		glVertex2f(texture.GetWidth(), texture.GetHeight());	// take top right and put at (xPos, yPos)
	glTexCoord2f(0, 1);		glVertex2f(0, texture.GetHeight());						// take bottom left and put at (xPos, yPos)
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Sprite::SpeedTo(float x) {
	speed = x;
}

void Sprite::SpeedBy(float x) {
	speed += x;
}

void Sprite::MoveTo(float x, float y) {
	xPos = x;
	yPos = y;
}

void Sprite::MoveBy(float x, float y) {
	xPos += x * Engine::GetDT();
	yPos += y * Engine::GetDT();
}

void Sprite::MoveLeft() {
	xPos -= speed * Engine::GetDT();
}

void Sprite::MoveRight() {
	xPos += speed * Engine::GetDT();
}

void Sprite::MoveUp() {
	yPos += speed * Engine::GetDT();
}

void Sprite::MoveDown() {
	yPos -= speed * Engine::GetDT();
}

void Sprite::RotateTo(float x) {
	rot = x;
}

void Sprite::RotateBy(float x) {
	rot += x * Engine::GetDT();
}

void Sprite::SetScale(float x) {
	xScale = x;
	yScale = x;
}

void Sprite::SetScale(float x, float y) {
	xScale = x;
	yScale = y;
}