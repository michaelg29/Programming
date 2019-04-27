#include "Sprite.h"
#include "../Engine.h"

Sprite::Sprite() {
	texture = Texture();
	pos = Vector3(0);
	rot = 0;
	speed = 100;
	scale = Vector3(1);
	size = Vector3(0);
}

Sprite::Sprite(std::string imagePath) :
	Sprite() {
	texture = Texture(imagePath);
	size = Vector3((float)texture.GetWidth(), (float)texture.GetHeight(), 1);
}

Sprite::Sprite(std::string imagePath, Vector3 v) :
	Sprite() {
	texture = Texture(imagePath);
	pos = v;
	size = Vector3((float)texture.GetWidth(), (float)texture.GetHeight(), 1);
}

void Sprite::Update() {
	
}

void Sprite::Render() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.GetID());
	glLoadIdentity();

	// translate -> rotate -> scale
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(rot, 0, 0, 1);
	glScalef(scale.x, scale.y, 1);

	// rendering
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	{
		// take each corner and put it at a scene coordinate; relative to translation
		glTexCoord2f(0, 0);		glVertex2i(-texture.GetWidth() / 2, -texture.GetHeight() / 2);										// take bottom left and put at (xPos, yPos)
		glTexCoord2f(1, 0);		glVertex2i( texture.GetWidth() / 2, -texture.GetHeight() / 2);						// take bottom right and put at (xPos, yPos)
		glTexCoord2f(1, 1);		glVertex2i( texture.GetWidth() / 2, texture.GetHeight() / 2);	// take top right and put at (xPos, yPos)
		glTexCoord2f(0, 1);		glVertex2i(-texture.GetWidth() / 2, texture.GetHeight() / 2);						// take bottom left and put at (xPos, yPos)
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Sprite::SpeedTo(float x) {
	speed = x;
}

void Sprite::SpeedBy(float x) {
	speed += x;
}

void Sprite::MoveTo(Vector3 v) {
	pos = v;
}

void Sprite::MoveBy(Vector3 v) {
	pos = pos + v * Engine::GetDT();
}

void Sprite::MoveLeft() {
	pos = pos - Vector3(speed * Engine::GetDT(), 0, 0);
}

void Sprite::MoveRight() {
	pos = pos + Vector3(speed * Engine::GetDT(), 0, 0);
}

void Sprite::MoveUp() {
	pos = pos + Vector3(0, speed * Engine::GetDT(), 0);
}

void Sprite::MoveDown() {
	pos = pos - Vector3(0, speed * Engine::GetDT(), 0);
}

void Sprite::RotateTo(float x) {
	rot = x;
}

void Sprite::RotateBy(float x) {
	rot += x * Engine::GetDT();
}

void Sprite::SetScale(float x) {
	scale = Vector3(x);
}

void Sprite::SetScale(Vector3 v) {
	scale = v;
}

Vector3* Sprite::GetPos() {
	return &pos;
}

float* Sprite::GetRot() {
	return &rot;
}

Vector3* Sprite::GetScale() {
	return &scale;
}

Vector3* Sprite::GetSize() {
	return &size;
}