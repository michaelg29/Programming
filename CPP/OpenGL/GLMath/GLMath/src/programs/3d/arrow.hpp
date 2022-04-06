#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "../program.h"
#include "../shader.h"
#include "../vertexmemory.hpp"
#include "../uniformmemory.hpp"
#include "../../io/camera.h"
#include "../../io/keyboard.h"
#include "../../io/mouse.h"

#ifndef ARROW_HPP
#define ARROW_HPP

class Arrow : public Program {
	Shader shader;

	unsigned int maxNoInstances;
	unsigned int noInstances;

	std::vector<glm::vec3> startPts;
	std::vector<glm::vec3> endPts;
	std::vector<glm::vec3> dimensions;
	std::vector<glm::vec3> colors;
	std::vector<glm::mat4> mats;

	ArrayObject VAO;

	Camera cam;
	glm::mat4 view;
	glm::mat4 projection;

public:
	Arrow(unsigned int maxNoInstances)
		: maxNoInstances(maxNoInstances), noInstances(0), view(1.0f), projection(1.0f) {}

	void load() {
		shader = Shader(false, "3d/arrow.vs", "3d/arrow.fs", "3d/arrow.gs");

		VAO.generate();
		VAO.bind();

		// load data into vertex buffers
		VAO["startVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["startVBO"].generate();
		VAO["startVBO"].bind();
		VAO["startVBO"].setData<glm::vec3>(noInstances, &startPts[0], GL_STATIC_DRAW);
		VAO["startVBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 3, 0);
		VAO["startVBO"].clear();

		VAO["endVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["endVBO"].generate();
		VAO["endVBO"].bind();
		VAO["endVBO"].setData<glm::vec3>(noInstances, &endPts[0], GL_STATIC_DRAW);
		VAO["endVBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 3, 0);

		VAO["radiiVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["radiiVBO"].generate();
		VAO["radiiVBO"].bind();
		VAO["radiiVBO"].setData<glm::vec3>(noInstances, &dimensions[0], GL_STATIC_DRAW);
		VAO["radiiVBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 3, 0);

		VAO["colorVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["colorVBO"].generate();
		VAO["colorVBO"].bind();
		VAO["colorVBO"].setData<glm::vec3>(noInstances, &colors[0], GL_STATIC_DRAW);
		VAO["colorVBO"].setAttPointer<GLfloat>(3, 3, GL_FLOAT, 3, 0);

		VAO["matVBO"] = BufferObject(GL_ARRAY_BUFFER);
		VAO["matVBO"].generate();
		VAO["matVBO"].bind();
		VAO["matVBO"].setData<glm::mat4>(noInstances, &mats[0], GL_STATIC_DRAW);
		VAO["matVBO"].setAttPointer<glm::vec4>(4, 4, GL_FLOAT, 4, 0);
		VAO["matVBO"].setAttPointer<glm::vec4>(5, 4, GL_FLOAT, 4, 1);
		VAO["matVBO"].setAttPointer<glm::vec4>(6, 4, GL_FLOAT, 4, 2);
		VAO["matVBO"].setAttPointer<glm::vec4>(7, 4, GL_FLOAT, 4, 3);

		
	}

	bool addInstance(glm::vec3 start, glm::vec3 end, float radius, float head_radius, float head_height, glm::vec3 color) {
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
			// arrow goes nowhere
			return false;
		}

		mat[0] = glm::vec4(b1, 0.0f);
		mat[1] = glm::vec4(armVector, 0.0f);
		mat[2] = glm::vec4(b2, 0.0f);
		mat[3] = glm::vec4(start, 1.0f);
		mats.push_back(mat);

		startPts.push_back(start);
		endPts.push_back(end);
		dimensions.push_back({ radius, head_radius, head_height });
		colors.push_back(color);

		noInstances++;

		return true;
	}

	void updateCameraMatrices(glm::mat4 view, glm::mat4 projection) {
		shader.activate();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
	}

	void render(double dt) {
		shader.activate();
		VAO.bind();
		VAO.draw(GL_POINTS, 0, noInstances);
	}

	void cleanup() {
		noInstances = 0;

		startPts.clear();
		endPts.clear();
		dimensions.clear();

		shader.cleanup();
		VAO.cleanup();
	}
};

#endif // ARROW_HPP