#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "program.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/vertexmemory.hpp"
#include "../rendering/uniformmemory.hpp"

#ifndef ARROW_HPP
#define ARROW_HPP

class Arrow : public Program {
	unsigned int maxNoInstances;
	unsigned int noInstances;

	std::vector<glm::vec4> dimensions; // magnitude, radius, head_radius, head_height
	std::vector<glm::vec4> colors; // rgb, shininess
	std::vector<glm::mat4> mats;
	std::vector<glm::vec3> diffuse;
	std::vector<glm::vec3> specular;

	ArrayObject VAO;

public:
	Arrow(unsigned int maxNoInstances)
		: maxNoInstances(maxNoInstances), noInstances(0) {}

	void load() {
		shader = Shader(false, "3d/arrow.vs", "3d/dirlight.fs", "3d/arrow.gs");

		VAO.generate();
		VAO.bind();

		// load data into vertex buffers
		VAO["dimVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["dimVBO"].generate();
		VAO["dimVBO"].bind();
		VAO["dimVBO"].setData<glm::vec4>(noInstances, &dimensions[0], GL_STATIC_DRAW);
		VAO["dimVBO"].setAttPointer<GLfloat>(0, 4, GL_FLOAT, 4, 0);

		VAO["colorVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["colorVBO"].generate();
		VAO["colorVBO"].bind();
		VAO["colorVBO"].setData<glm::vec4>(noInstances, &colors[0], GL_STATIC_DRAW);
		VAO["colorVBO"].setAttPointer<GLfloat>(1, 4, GL_FLOAT, 4, 0);

		VAO["matVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["matVBO"].generate();
		VAO["matVBO"].bind();
		VAO["matVBO"].setData<glm::mat4>(noInstances, &mats[0], GL_STATIC_DRAW);
		VAO["matVBO"].setAttPointer<glm::vec4>(2, 4, GL_FLOAT, 4, 0);
		VAO["matVBO"].setAttPointer<glm::vec4>(3, 4, GL_FLOAT, 4, 1);
		VAO["matVBO"].setAttPointer<glm::vec4>(4, 4, GL_FLOAT, 4, 2);
		VAO["matVBO"].setAttPointer<glm::vec4>(5, 4, GL_FLOAT, 4, 3);
	
		VAO["diffVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["diffVBO"].generate();
		VAO["diffVBO"].bind();
		VAO["diffVBO"].setData<glm::vec3>(noInstances, &diffuse[0], GL_STATIC_DRAW);
		VAO["diffVBO"].setAttPointer<GLfloat>(6, 3, GL_FLOAT, 3, 0);

		VAO["specularVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["specularVBO"].generate();
		VAO["specularVBO"].bind();
		VAO["specularVBO"].setData<glm::vec3>(noInstances, &specular[0], GL_STATIC_DRAW);
		VAO["specularVBO"].setAttPointer<GLfloat>(7, 3, GL_FLOAT, 3, 0);
	}

	bool addInstance(glm::vec3 start, glm::vec3 end, float radius, float head_radius, float head_height, Material material) {
		if (noInstances >= maxNoInstances)
		{
			return false;
		}

		glm::mat4 mat(1.0f);
		glm::vec3 armVector = glm::normalize(end - start); // {ax, ay, az}

		// arm vector perpendicular to plane with equation ax*x + ay*y + az*z = 0
		// get basis of the plane
		glm::vec3 b1, b2;

		if (armVector.y != 0.0f)
		{
			// y = -(ax*x + az*z)/ay
			b1 = glm::normalize(glm::vec3(1.0f, -armVector.x / armVector.y, 0.0f));
			b2 = glm::normalize(glm::vec3(0.0f, -armVector.z / armVector.y, 1.0f));
		}
		else if (armVector.z != 0.0f)
		{
			// z = -(ax*x + ay*y)/az
			b1 = glm::normalize(glm::vec3(1.0f, 0.0f, -armVector.x / armVector.z));
			b2 = glm::normalize(glm::vec3(0.0f, 1.0f, -armVector.y / armVector.z));
		}
		else if (armVector.x != 0.0f)
		{
			// x = -(ay*y + az*z)/ax
			b1 = glm::normalize(glm::vec3(-armVector.y / armVector.x, 1.0f, 0.0f));
			b2 = glm::normalize(glm::vec3(-armVector.z / armVector.x, 0.0f, 1.0f));
		}
		else
		{
			// two points are the same
			return false;
		}

		/*
			in geometry shader, arrow drawn with base in XZ plane (y = 0) and arm along y-axis
			transform y unit vector to be along the armVector
			transform x/z unit vectors to be in the plane of the base, perpendicular to the arm
		*/
		mat[0] = glm::vec4(b1, 0.0f); // how x unit vector gets transformed
		mat[1] = glm::vec4(armVector, 0.0f); // how y unit vector gets transformed
		mat[2] = glm::vec4(b2, 0.0f); // how z unit vector gets transformed
		mat[3] = glm::vec4(start, 1.0f); // translation to start point
		mats.push_back(mat);

		dimensions.push_back({ glm::length(end - start), radius, head_radius, head_height });
		colors.push_back(glm::vec4(material.ambient, material.shininess));
		diffuse.push_back(material.diffuse);
		specular.push_back(material.specular);

		noInstances++;

		return true;
	}

	void render(double dt) {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_POINTS, 0, noInstances);
	}

	void cleanup() {
		noInstances = 0;

		dimensions.clear();
		colors.clear();
		mats.clear();

		shader.cleanup();
		VAO.cleanup();
	}
};

#endif // ARROW_HPP