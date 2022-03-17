#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "../program.h"
#include "../shader.h"
#include "../vertexmemory.hpp"
#include "../uniformmemory.hpp"

#ifndef GRID2D_HPP
#define GRID2D_HPP

class Grid2D : public Program {
	float lineIncrement_x;
	float lineIncrement_y;
	glm::vec3 color_x;
	glm::vec3 color_y;

	Shader shader;
	GLuint VAO;
	UBO::UBO ubo;

public:

	Grid2D(glm::vec3 color_x, glm::vec3 color_y, float lineIncrement_x, float lineIncrement_y)
		: lineIncrement_x(lineIncrement_x), lineIncrement_y(lineIncrement_y),
		color_x(color_x),
		color_y(color_y),
		ubo(0, {
			UBO::newArray(2, UBO::newStruct({
				UBO::Type::SCALAR,
				UBO::Type::VEC3
			}))
		}) {}

	Grid2D(glm::vec3 color_x, glm::vec3 color_y, int noLines_x, int noLines_y)
		: Grid2D(color_x, color_y, 1.0f / (float)noLines_x, 1.0f / (float)noLines_y) {}

	void load() {
		shader = Shader(false, "grid.vs", "grid.fs", "grid.gs");

		ubo.attachToShader(shader, "Axes");

		// setup memory
		ubo.generate();
		ubo.bind();
		ubo.initNullData(GL_STATIC_DRAW);
		ubo.bindRange();

		// write values
		ubo.startWrite();
		ubo.writeElement<float>(&lineIncrement_x);
		ubo.writeElement<glm::vec3>(&color_x);
		ubo.writeElement<float>(&lineIncrement_y);
		ubo.writeElement<glm::vec3>(&color_y);
		ubo.clear();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
	}

	void processInput(double dt, GLFWwindow* window) {}

	void render(double dt) {
		glBindVertexArray(VAO);
		shader.activate();
		glDrawArrays(GL_POINTS, 0, 2);
	}

	void cleanup() {
		ubo.cleanup();
		glDeleteVertexArrays(1, &VAO);

		shader.cleanup();
	}
};

#endif