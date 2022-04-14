#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "program.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/vertexmemory.hpp"
#include "../rendering/uniformmemory.hpp"

#ifndef SURFACE_HPP
#define SURFACE_HPP

class Surface : public Program {
	ArrayObject VAO;
	int x_len;
	int y_len;
	glm::vec4 bounds;
	glm::vec4 dim; // x_inc, y_inc, z_min, z_max
	Material material;
	unsigned int noPoints;
	UBO::UBO uniformObject;

public:
	Surface(glm::vec2 start, glm::vec2 end, int x_cells, int z_cells, float minY, float maxY, Material material) 
		: uniformObject({
			UBO::newVec(4), // startXZ, endXZ
			UBO::newVec(4), // x_inc, z_inc, y_min, y_max
			UBO::newVec(3), // ambient
			UBO::newVec(3), // diffuse
			UBO::newVec(3), // specular
			UBO::newScalar() // shininess
			}),
			bounds(start, end), material(material), noPoints(x_cells * z_cells),
			x_len(x_cells), y_len(z_cells) {
		float x_inc = (end.x - start.x) / (float)x_cells;
		float y_inc = (end.y - start.y) / (float)z_cells;
		dim = glm::vec4(x_inc, y_inc, minY, maxY);
	}

	void load() {
		shader = Shader(false, "3d/surface.vs", "3d/dirlight.fs", "3d/surface.gs");
		shader.activate();
		shader.setInt("x_len", x_len);
		shader.setInt("y_len", y_len);

		VAO.generate();

		uniformObject.attachToShader(shader, "SurfaceUniform");
		// generate/bind
		uniformObject.generate();
		uniformObject.bind();
		uniformObject.initNullData(GL_STATIC_DRAW);
		uniformObject.bindRange();

		uniformObject.startWrite();
		uniformObject.writeElement<glm::vec4>(&bounds);
		uniformObject.writeElement<glm::vec4>(&dim);
		uniformObject.writeElement<glm::vec3>(&material.ambient);
		uniformObject.writeElement<glm::vec3>(&material.diffuse);
		uniformObject.writeElement<glm::vec3>(&material.specular);
		uniformObject.writeElement<float>(&material.shininess);
	}

	void render(double dt) {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_POINTS, 0, noPoints);
	}

	void cleanup() {
		shader.cleanup();
		VAO.cleanup();
		uniformObject.cleanup();
	}
};

#endif // SURFACE_HPP