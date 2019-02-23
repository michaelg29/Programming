#include "Sprite.h"

Sprite::Sprite() {
	texture = Texture();
	xPos = 0;
	yPos = 0;
}

Sprite::Sprite(std::string imagePath) {
	texture = Texture(imagePath);
	xPos = 0;
	yPos = 0;
}

Sprite::Sprite(std::string imagePath, float _xPos, float _yPos) {
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
	//glRotate
	//glScale

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