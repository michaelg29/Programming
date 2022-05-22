#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "program.h"
#include "../io/keyboard.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/transition.hpp"
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
	std::vector<glm::mat3> normalMats;
	std::vector<glm::vec3> diffuse;
	std::vector<glm::vec3> specular;

	ArrayObject VAO;

	CubicBezier<glm::vec3> transition;
	bool transitionStarted;

public:
	Arrow(unsigned int maxNoInstances)
		: maxNoInstances(maxNoInstances), noInstances(0),
		transition(CubicBezier<glm::vec3>::newEaseTransition(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f), 3.0)),
		transitionStarted(false) {}

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
	
		VAO["normMatVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["normMatVBO"].generate();
		VAO["normMatVBO"].bind();
		VAO["normMatVBO"].setData<glm::mat3>(noInstances, &normalMats[0], GL_STATIC_DRAW);
		VAO["normMatVBO"].setAttPointer<glm::vec3>(6, 3, GL_FLOAT, 3, 0);
		VAO["normMatVBO"].setAttPointer<glm::vec3>(7, 3, GL_FLOAT, 3, 1);
		VAO["normMatVBO"].setAttPointer<glm::vec3>(8, 3, GL_FLOAT, 3, 2);

		VAO["diffVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["diffVBO"].generate();
		VAO["diffVBO"].bind();
		VAO["diffVBO"].setData<glm::vec3>(noInstances, &diffuse[0], GL_STATIC_DRAW);
		VAO["diffVBO"].setAttPointer<GLfloat>(9, 3, GL_FLOAT, 3, 0);

		VAO["specularVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["specularVBO"].generate();
		VAO["specularVBO"].bind();
		VAO["specularVBO"].setData<glm::vec3>(noInstances, &specular[0], GL_STATIC_DRAW);
		VAO["specularVBO"].setAttPointer<GLfloat>(10, 3, GL_FLOAT, 3, 0);
	}

	void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (Keyboard::keyWentDown(GLFW_KEY_Y)) {
			transitionStarted = !transitionStarted;
		}
	}

	glm::mat4 calculateModelMatrix(glm::vec3 start, glm::vec3 armVector) {
		glm::mat4 mat(1.0f);
		armVector = glm::normalize(armVector); // {ax, ay, az}

		// arm vector perpendicular to plane with equation ax*x + ay*y + az*z = 0
		// get basis of the plane
		glm::vec3 b1, b2;

		if (armVector.z != 0.0f)
		{
			// z = -(ax*x + ay*y)/az
			b1 = glm::normalize(glm::vec3(1.0f, 0.0f, -armVector.x / armVector.z));
			b2 = glm::normalize(glm::vec3(0.0f, 1.0f, -armVector.y / armVector.z));
		}
		else if (armVector.y != 0.0f)
		{
			// y = -(ax*x + az*z)/ay
			b1 = glm::normalize(glm::vec3(1.0f, -armVector.x / armVector.y, 0.0f));
			b2 = glm::normalize(glm::vec3(0.0f, -armVector.z / armVector.y, 1.0f));
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
			return mat;
		}

		// orthogonalize b2 with respect to b1, armVector
		// b2 = glm::normalize(glm::cross(armVector, b1)); // more expensive operation
		b2 = glm::normalize(b2 - b1 * glm::dot(b1, b2)); // orthogonalization

		/*
			in geometry shader, arrow drawn with base in XZ plane (y = 0) and arm along y-axis
			transform y unit vector to be along the armVector
			transform x/z unit vectors to be in the plane of the base, perpendicular to the arm
		*/
		mat[0] = glm::vec4(b1, 0.0f); // how x unit vector gets transformed
		mat[1] = glm::vec4(armVector, 0.0f); // how y unit vector gets transformed
		mat[2] = glm::vec4(b2, 0.0f); // how z unit vector gets transformed
		mat[3] = glm::vec4(start, 1.0f); // translation to start point
		
		return mat;
	}

	bool addInstance(glm::vec3 start, glm::vec3 end, float radius, float head_radius, float head_height, Material material) {
		if (noInstances >= maxNoInstances || start == end)
		{
			return false;
		}

		glm::vec3 arm = end - start;
		glm::mat4 mat = calculateModelMatrix(start, arm);
		mats.push_back(mat);
		normalMats.push_back(glm::transpose(glm::inverse(mat)));

		dimensions.push_back({ glm::length(arm), radius, head_radius, head_height });
		colors.push_back(glm::vec4(material.ambient, material.shininess));
		diffuse.push_back(material.diffuse);
		specular.push_back(material.specular);

		noInstances++;

		return true;
	}

	bool update(double dt) {
		if (transitionStarted && noInstances) {
			glm::vec3 end = transition.update(dt);

			dimensions[0].x = glm::length(end);
			VAO["dimVBO"].bind();
			VAO["dimVBO"].updateData<glm::vec4>(0, noInstances, &dimensions[0]);

			mats[0] = calculateModelMatrix(glm::vec3(0.0f), end);
			VAO["matVBO"].bind();
			VAO["matVBO"].updateData<glm::mat4>(0, noInstances, &mats[0]);

			normalMats[0] = glm::transpose(glm::inverse(mats[0]));
			VAO["normMatVBO"].bind();
			VAO["normMatVBO"].updateData<glm::mat3>(0, noInstances, &normalMats[0]);

			return true;
		}

		return false;
	}

	void render() {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_POINTS, 0, noInstances);
	}

	void cleanup() {
		noInstances = 0;

		dimensions.clear();
		colors.clear();
		diffuse.clear();
		specular.clear();
		mats.clear();
		normalMats.clear();

		shader.cleanup();
		VAO.cleanup();
	}
};

#endif // ARROW_HPP