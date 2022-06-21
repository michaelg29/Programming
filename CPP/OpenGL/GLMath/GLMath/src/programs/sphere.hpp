#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "program.h"
#include "../io/keyboard.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/vertexmemory.hpp"
#include "../rendering/uniformmemory.hpp"
#include "../rendering/transition.hpp"

#ifndef SPHERE_HPP
#define SPHERE_HPP

class Sphere : public Program {
	unsigned int maxNoInstances;
	unsigned int noInstances;

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	std::vector<glm::vec3> offsets;
	std::vector<glm::vec3> sizes;
	std::vector<glm::vec3> diffuse;
	std::vector<glm::vec4> specular;

	ArrayObject VAO;

	LinearTransition<glm::vec3> transition;
	bool transitionStarted;

public:
	Sphere(unsigned int maxNoInstances)
		: maxNoInstances(maxNoInstances), noInstances(0),
		  //transition(CubicBezier<glm::vec3>::newEaseTransition(glm::vec3(0.0f), glm::vec3(1.0f), 1.0)),
		transition(LinearTransition<glm::vec3>(glm::vec3(0.0f), glm::vec3(1.0f), 2.0)),
		transitionStarted(false) {}

	void load() {
		shader = Shader(false, "3d/sphere.vert", "3d/dirlight.frag");

		// generate vertices
		unsigned int res = 100;
		float resF = (float)res; // number of rows, number of cells per row
		float circleStep = glm::two_pi<float>() / resF; // angle step between cells
		float heightStep = glm::pi<float>() / resF; // height of row
		
		int row = 0;
		int noVertices = 0;
		float height = -glm::half_pi<float>() + heightStep;
		float y = glm::sin(height);
		float radius = glm::cos(height);
		// generate first row
		for (float ang = 0; ang < glm::two_pi<float>(); ang += circleStep) {
			vertices.push_back(glm::vec3(radius * glm::cos(ang), y, radius * glm::sin(ang)));
			noVertices++;
		}

		// generate rest of rows
		height += heightStep;
		row++;
		for (; height < glm::half_pi<float>(); height += heightStep, row++) {
			y = glm::sin(height);
			radius = glm::cos(height);
			int cell = 0;
			for (float ang = 0; ang < glm::two_pi<float>(); ang += circleStep, cell++) {
				vertices.push_back(glm::vec3(radius * glm::cos(ang), y, radius * glm::sin(ang)));
				noVertices++;

				// add indices
				int nextCell = (cell + 1) % res;
				indices.push_back((row - 1) * res + cell); // this cell in prev row
				indices.push_back((row - 1) * res + nextCell); // next cell in prev row
				indices.push_back(row * res + cell); // this vertex

				indices.push_back(row * res + cell); // this vertex
				indices.push_back(row * res + nextCell); // next vertex in row
				indices.push_back((row - 1) * res + nextCell); // next cell in prev row
			}
		}

		// add poles
		vertices.push_back(glm::vec3(0.0f, -1.0f, 0.0f)); // south pole
		int sp_idx = noVertices;
		vertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); // north pole
		int np_idx = noVertices + 1;
		for (unsigned int i = 0; i < res; i++) {
			// south pole
			indices.push_back(sp_idx);
			indices.push_back(i); // current vertex
			indices.push_back((i + 1) % res); // next vertex

			// north pole
			indices.push_back(np_idx);
			indices.push_back(noVertices - 1 - i); // current vertex
			indices.push_back(noVertices - 1 - ((i + 1) % res)); // next vertex
		}

		// setup VAO
		VAO.generate();
		VAO.bind();

		// load data into vertex buffers
		VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["VBO"].generate();
		VAO["VBO"].bind();
		VAO["VBO"].setData<glm::vec3>((GLuint)vertices.size(), &vertices[0], GL_STATIC_DRAW);
		VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 3, 0);

		VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
		VAO["EBO"].generate();
		VAO["EBO"].bind();
		VAO["EBO"].setData<GLuint>((GLuint)indices.size(), &indices[0], GL_STATIC_DRAW);

		if (noInstances > 0)
		{
			VAO["offsetVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["offsetVBO"].generate();
			VAO["offsetVBO"].bind();
			VAO["offsetVBO"].setData<glm::vec3>(noInstances, &offsets[0], GL_DYNAMIC_DRAW);
			VAO["offsetVBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 3, 0, 1);

			VAO["sizeVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["sizeVBO"].generate();
			VAO["sizeVBO"].bind();
			VAO["sizeVBO"].setData<glm::vec3>(noInstances, &sizes[0], GL_STATIC_DRAW);
			VAO["sizeVBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 3, 0, 1);

			VAO["diffVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["diffVBO"].generate();
			VAO["diffVBO"].bind();
			VAO["diffVBO"].setData<glm::vec3>(noInstances, &diffuse[0], GL_STATIC_DRAW);
			VAO["diffVBO"].setAttPointer<GLfloat>(3, 3, GL_FLOAT, 3, 0, 1);

			VAO["specularVBO"] = BufferObject(GL_ARRAY_BUFFER);
			VAO["specularVBO"].generate();
			VAO["specularVBO"].bind();
			VAO["specularVBO"].setData<glm::vec4>(noInstances, &specular[0], GL_STATIC_DRAW);
			VAO["specularVBO"].setAttPointer<GLfloat>(4, 4, GL_FLOAT, 4, 0, 1);
		}
	}

	bool addInstance(glm::vec3 offset, glm::vec3 size, Material material) {
		if (noInstances >= maxNoInstances)
		{
			return false;
		}

		offsets.push_back(offset);
		sizes.push_back(size);
		diffuse.push_back(material.diffuse);
		specular.push_back(glm::vec4(material.specular, material.shininess));

		noInstances++;

		return true;
	}

	void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (Keyboard::keyWentDown(GLFW_KEY_T)) {
			transitionStarted = !transitionStarted;
		}
	}

	bool update(double dt) {
		if (transitionStarted && noInstances) {
			offsets[0] = transition.update(dt);
			VAO["offsetVBO"].bind();
			VAO["offsetVBO"].updateData<glm::vec3>(0, noInstances, &offsets[0]);
			return true;
		}

		return false;
	}

	void render() {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0, noInstances);
	}

	void cleanup() {
		noInstances = 0;

		offsets.clear();
		sizes.clear();
		diffuse.clear();
		specular.clear();

		shader.cleanup();
		VAO.cleanup();
	}
};

#endif // SPHERE_HPP