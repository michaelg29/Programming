#ifndef BOUNDS_HPP
#define BOUNDS_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath>

#include "../shader.h"

#include <glm/glm.hpp>
#include <vector>

class Bounds {
public:
	std::vector<glm::vec3> offsets;
	std::vector<glm::vec3> sizes;

	void init() {
		vertices = {
			// position				x	y	z	i
			 0.5f,  0.5f,  0.5f, //	+	+	+	0
			-0.5f,  0.5f,  0.5f, //	-	+	+	1
			-0.5f, -0.5f,  0.5f, //	-	-	+	2
			 0.5f, -0.5f,  0.5f, //	+	-	+	3
			 0.5f,  0.5f, -0.5f, //	+	+	-	4
			-0.5f,  0.5f, -0.5f, //	-	+	-	5
			-0.5f, -0.5f, -0.5f, //	-	-	-	6
			 0.5f, -0.5f, -0.5f	 //	+	-	-	7
		};

		indices = { // 12 lines
			// front face (+ve z)
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			// back face (-ve z)
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			// right face (+ve x)
			0, 4,
			3, 7,
			// left face (-ve x)
			1, 5,
			2, 6
		};

		// generate VAO
		glGenVertexArrays(1, &VAO);

		// generate vertices VBO
		glGenBuffers(1, &verticesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		/*
			Allocate memory for max of 100 boxes
		*/

		// generate offset VBO - dynamic
		glGenBuffers(1, &offsetVBO);
		glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
		glBufferData(GL_ARRAY_BUFFER, 100 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate size VBO - dynamic
		glGenBuffers(1, &sizeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
		glBufferData(GL_ARRAY_BUFFER, 100 * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		glBindVertexArray(VAO);
		// vertex Positions
		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// offset
		glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		// size
		glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glVertexAttribDivisor(1, 1); // reset _1st_ attribute every _1_ instance
		glVertexAttribDivisor(2, 1); // reset _2nd_ attribute every _1_ instance
	
		glBindVertexArray(0);
	}

	void render(Shader shader) {
		/*for (int i = 0; i < vertices.size(); i += 3) {
			vertices[i] += 0.01f;
		}*/
		/*
		// glMapBuffer

		glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
		// get pointer to memory
		void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		// copy new data into memory
		memcpy(ptr, &vertices[0], sizeof(vertices));
		// tell OpenGL we are done
		glUnmapBuffer(GL_ARRAY_BUFFER);
		*/

		// glBufferSubData
		if (offsets.size() != 0) {
			glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, std::min(100, (int)offsets.size()) * 3 * sizeof(float), &offsets[0]);

			glBindBuffer(GL_ARRAY_BUFFER, sizeVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, std::min(100, (int)sizes.size()) * 3 * sizeof(float), &sizes[0]);
		}

		shader.setMat4("model", glm::mat4(1.0f));

		glBindVertexArray(VAO);
		//glDrawArraysInstanced(GL_LINES, 0, 36, 1);
		//glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
		glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, offsets.size());
		glBindVertexArray(0);
	}

	void cleanup() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteVertexArrays(1, &verticesVBO);
		glDeleteVertexArrays(1, &offsetVBO);
		glDeleteVertexArrays(1, &sizeVBO);
	}

private:
	unsigned int VAO;
	unsigned int verticesVBO, offsetVBO, sizeVBO;
	unsigned int EBO;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

#endif