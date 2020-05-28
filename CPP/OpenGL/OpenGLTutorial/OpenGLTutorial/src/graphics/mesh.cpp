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

Mesh::Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : br(br), vertices(vertices), indices(indices), textures(textures), noTex(false) {
    setup();
}

Mesh::Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec)
    : br(br), vertices(vertices), indices(indices), diff(diff), spec(spec), noTex(true) {
    setup();
}

void Mesh::render(Shader shader, glm::vec3 pos, glm::vec3 size, Box *b, bool doRender) {
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
        b->offsets.push_back(pos + br.calculateCenter());
        b->sizes.push_back(size * br.calculateDimensions());

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }
}

void Mesh::setup() {
    // bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // load data into vertex buffers
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // normal ray
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normal)));
    glEnableVertexAttribArray(1);
    // vertex texture coords
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Mesh::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
