#include "mesh.h"
#include "../io/camera.h"

#include <iostream>

std::vector<Vertex> Vertex::genList(float* vertices, int noVertices) {
	std::vector<Vertex> ret(noVertices);

	int stride = 8;

	for (int i = 0; i < noVertices; i++) {
		ret[i].pos = glm::vec3(
			vertices[i * stride],
			vertices[i * stride + 1],
			vertices[i * stride + 2]
		);

        ret[i].normal = glm::vec3(
            vertices[i * stride + 3],
            vertices[i * stride + 4],
            vertices[i * stride + 5]
        );

		ret[i].texCoord = glm::vec2(
			vertices[i * stride + 6],
			vertices[i * stride + 7]
		);
	}

    return ret;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures), noTex(false) {
    setup();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec)
    : vertices(vertices), indices(indices), diff(diff), spec(spec), noTex(true) {
    setup();
}

void Mesh::render(Shader shader, glm::vec3 pos, bool doRender) {
    if (noTex) {
        shader.set4Float("material.diffuse", diff);
        shader.set4Float("material.specular", spec);
        shader.setInt("noTex", 1);
    }
    else {
        unsigned int diffuseIdx = 0;
        unsigned int specularIdx = 0;

        for (unsigned int i = 0; i < textures.size(); i++) {
            // active proper texture unit before binding
            glActiveTexture(GL_TEXTURE0 + i);

            // retrieve texture number (the N in diffuse_textureN)
            std::string name;
            switch (textures[i].type) {
            case aiTextureType_DIFFUSE:
                name = "diffuse" + std::to_string(diffuseIdx++);
                break;

            case aiTextureType_SPECULAR:
                name = "specular" + std::to_string(specularIdx++);
                break;
            };

            // now set the sampler to the correct texture unit
            shader.setInt(name, i);
            // and finally bind the texture
            textures[i].bind();
        }

        shader.setInt("noTex", 0);
    }

    if (doRender) {
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }
}

void Mesh::setup() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // normal ray
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    // vertex texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
