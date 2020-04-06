#ifndef CUBE_H
#define CUBE_H

#include "../model.h"

class Cube : public Model {
public:
	Texture tex1;
	Texture tex2;

	Mesh mesh;

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f);

	Cube(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f)) 
		: pos(pos), size(size) {}

	void init() {
		int noVertices = 36;

		float vertices[] = {
			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

			-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
		};
		
		std::vector<unsigned int> indices(noVertices);
		for (unsigned int i = 0; i < noVertices; i++) {
			indices[i] = i;
		}
		
		tex1 = Texture("assets/image1.jpg", "texture0");
		tex1.load();
		tex2 = Texture("assets/image2.png", "texture1");
		tex2.load();

		mesh = Mesh(Vertex::genList(vertices, noVertices), indices, { tex1, tex2 });
	}

	void render(Shader shader) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::scale(model, size);
		shader.setMat4("model", model);

		mesh.draw(shader);
	}
};

#endif