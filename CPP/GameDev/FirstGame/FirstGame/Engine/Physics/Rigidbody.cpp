#include "Rigidbody.h"

#include "../Engine.h"

Rigidbody::Rigidbody() {
	friction = 1;
	gravity = 0;
}

void Rigidbody::Initialize(float _friction, float _gravity, Vector3* _pos, float* _rot, Vector3* _scale, Vector3* _size) {
	friction = _friction;
	gravity = _gravity;
	
	pos = _pos;
	rot = _rot;
	scale = _scale;
	size = _size;
}

void Rigidbody::Update() {
	// change velocity
	vel.x *= friction;
	vel.y += gravity;

	*pos = *pos + vel * Engine::GetDT(); // dereference with *pos
}

void Rigidbody::Render(Vector3 c) {
	glLoadIdentity();

	glTranslatef(pos->x, pos->y, pos->z);
	glRotatef(*rot, 0, 0, 1);
	glScalef(scale->x, scale->y, scale->z);

	// rendering
	glColor4f(c.x, c.y, c.z, 1);
	glBegin(GL_LINES);
	{
		// take each corner and put it at a scene coordinate; relative to translation
		glVertex2i(-(int)size->x / 2, -(int)size->y / 2);			// take bottom left and put at (xPos, yPos)
		glVertex2i((int)size->x / 2, -(int)size->y / 2);			// take bottom right and put at (xPos, yPos)

		glVertex2i((int)size->x / 2, -(int)size->y / 2);			// take bottom right and put at (xPos, yPos)
		glVertex2i((int)size->x / 2, (int)size->y / 2);				// take top right and put at (xPos, yPos)

		glVertex2i((int)size->x / 2, (int)size->y / 2);				// take top right and put at (xPos, yPos)
		glVertex2i(-(int)size->x / 2, (int)size->y / 2);			// take bottom left and put at (xPos, yPos)

		glVertex2i(-(int)size->x / 2, (int)size->y / 2);			// take bottom left and put at (xPos, yPos)
		glVertex2i(-(int)size->x / 2, -(int)size->y / 2);			// take bottom left and put at (xPos, yPos)
	}

	glEnd();
}

void Rigidbody::AddForce(Vector3 force) {
	vel = vel + force;
}

Vector3 Rigidbody::GetVel() {
	return vel;
}

void Rigidbody::SetVel(Vector3 _vel) {
	vel = _vel;
}