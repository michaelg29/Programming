#ifndef MODELARRAY_HPP
#define MODELARRAY_HPP

#include "../model.h"

template <class T>
class ModelArray {
public:
	std::vector<RigidBody> instances;

	void init() {
		model.init();

		// generate positions VBO
		glGenBuffers(1, &posVBO);
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, 100 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate size VBO - dynamic
		glGenBuffers(1, &sizeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
		glBufferData(GL_ARRAY_BUFFER, 100 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		for (unsigned int i = 0, size = model.meshes.size(); i < size; i++) {
			glBindVertexArray(model.meshes[i].VAO);
			// set the vertex attribute pointers
			// positions
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(3);
			// size
			glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glVertexAttribDivisor(3, 1); // reset _3rd_ attribute every _1_ instance
			glVertexAttribDivisor(4, 1); // reset _4th_ attribute every _1_ instance

			glBindVertexArray(0);
		}
	}

	void render(Shader shader, float dt, bool setLists = true) {
		/*for (RigidBody& rb : instances) {
			rb.update(dt);
			model.rb.pos = rb.pos;
			model.render(shader, dt);
		}*/

		if (setLists) {
			positions.clear();
			sizes.clear();
			for (RigidBody& rb : instances) {
				rb.update(dt);
				positions.push_back(rb.pos);
				sizes.push_back(model.size);
			}
		}

		shader.setMat4("model", glm::mat4(1.0f));
		
		model.render(shader, dt, false, false);

		// reset VBO data
		/*if (positions.size() != 0) {
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * 3 * sizeof(float), &positions[0]);

			glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizes.size() * 3 * sizeof(float), &sizes[0]);
		}*/

		if (positions.size() != 0) {
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * 3 * sizeof(float), &positions[0]);

			glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizes.size() * 3 * sizeof(float), &sizes[0]);
		}

		for (unsigned int i = 0, size = model.meshes.size(); i < size; i++) {
			glBindVertexArray(model.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, positions.size());
			glBindVertexArray(0);
		}
	}

	void setSize(glm::vec3 size) {
		model.size = size;
	}

	void cleanup() {
		model.cleanup();
	}

protected:
	T model;

	unsigned int posVBO;
	unsigned int sizeVBO;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> sizes;
};

#endif