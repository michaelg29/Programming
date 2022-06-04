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
	int x_cells;
	int z_cells;
	glm::vec4 dim; // x_inc, y_inc, z_min, z_max
	Material material;
	unsigned int noPoints;
	UBO::UBO uniformObject;

	unsigned int noInstances;
	unsigned int maxNoInstances;
	std::vector<glm::vec4> bounds;
	std::vector<glm::vec2> inc;
	std::vector<glm::vec3> diffuse;
	std::vector<glm::vec4> specular; // rgb, shininess

public:
	Surface(unsigned int maxNoInstances, int x_cells, int z_cells)
		: maxNoInstances(maxNoInstances), noInstances(0), x_cells(x_cells), z_cells(z_cells), noPoints(x_cells * z_cells) {}

	bool addInstance(glm::vec2 start, glm::vec2 end, Material material) {
		if (noInstances >= maxNoInstances) {
			return false;
		}

		float x_inc = (end.x - start.x) / (float)x_cells;
		float z_inc = (end.y - start.y) / (float)z_cells;

		bounds.push_back(glm::vec4(start, end));
		inc.push_back(glm::vec2(x_inc, z_inc));
		diffuse.push_back(material.diffuse);
		specular.push_back(glm::vec4(material.specular, material.shininess));

		noInstances++;
		return true;
	}

	void load() {
		shader = Shader(false, "3d/surface.vert", "3d/dirlight.frag", "3d/surface.geom");
		shader.activate();
		shader.setInt("x_cells", x_cells);
		shader.setInt("z_cells", z_cells);

		VAO.generate();
		VAO.bind();

		if (noInstances) {
			VAO["boundsVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["boundsVBO"].generate();
			VAO["boundsVBO"].bind();
			VAO["boundsVBO"].setData<glm::vec4>(noInstances, &bounds[0], GL_STATIC_DRAW);
			VAO["boundsVBO"].setAttPointer<GLfloat>(0, 4, GL_FLOAT, 4, 0, 1);

			VAO["incVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["incVBO"].generate();
			VAO["incVBO"].bind();
			VAO["incVBO"].setData<glm::vec2>(noInstances, &inc[0], GL_STATIC_DRAW);
			VAO["incVBO"].setAttPointer<GLfloat>(1, 2, GL_FLOAT, 2, 0, 1);

			VAO["diffVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["diffVBO"].generate();
			VAO["diffVBO"].bind();
			VAO["diffVBO"].setData<glm::vec3>(noInstances, &diffuse[0], GL_STATIC_DRAW);
			VAO["diffVBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 3, 0, 1);

			VAO["specularVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["specularVBO"].generate();
			VAO["specularVBO"].bind();
			VAO["specularVBO"].setData<glm::vec4>(noInstances, &specular[0], GL_STATIC_DRAW);
			VAO["specularVBO"].setAttPointer<GLfloat>(3, 4, GL_FLOAT, 4, 0, 1);
		}
	}

	void render() {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_POINTS, 0, noPoints, noInstances);
	}

	void cleanup() {
		shader.cleanup();
		VAO.cleanup();
		uniformObject.cleanup();
	}
};

#endif // SURFACE_HPP