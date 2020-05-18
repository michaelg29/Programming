#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "../shader.h"
#include <glm/glm.hpp>

class Shapes {
public:
	void setup(Shader shader) {
		glGenBuffers(1, &VBO);

		float points[] = {
			-0.45f, 0.45f, 0.0f,
			0.45f, 0.45f, 0.0f,
			0.45f, -0.45f, 0.0f,
			-0.45f, -0.45f, 0.0f
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void render(Shader shader) {
		shader.activate();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f));

		shader.setMat4("model", model);

		glBindVertexArray(VAO);
		glDrawArrays(GL_POINTS, 0, 4);
	}

	void cleanup() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &VBO);
	}

private:
	unsigned int VBO, VAO;
};

#endif