#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "program.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/vertexmemory.hpp"
#include "../rendering/uniformmemory.hpp"

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

class Rectangle : public Program {
	ArrayObject VAO;
	float points[18] = {
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f
	};

public:
	void load() {
		shader = Shader(false, "2d/rectangle.vs", "2d/rectangle.fs");

		VAO.generate();
		VAO.bind();
		VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["VBO"].generate();
		VAO["VBO"].bind();
		VAO["VBO"].setData<GLfloat>(18, points, GL_STATIC_DRAW);
		VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 3, 0);
	}

	void render(double dt) {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_TRIANGLES, 0, 6);
	}

	void cleanup() {
		shader.cleanup();
		VAO.cleanup();
	}
};

#endif // RECTANGLE_HPP