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
        b->positions.push_back(pos + br.calculateCenter());
        b->sizes.push_back(size * br.calculateDimensions());

        VAO.bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        ArrayObject::clear();

        glActiveTexture(GL_TEXTURE0);
    }
}

void Mesh::setup() {
    // bind VAO
    VAO.generate();
    VAO.bind();

    // generate/set EBO
    VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
    VAO["EBO"].generate();
    VAO["EBO"].bind();
    VAO["EBO"].setData<GLuint>(indices.size(), &indices[0], GL_STATIC_DRAW);

    // load data into vertex buffers
    VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
    VAO["VBO"].generate();
    VAO["VBO"].bind();
    VAO["VBO"].setData<Vertex>(vertices.size(), &vertices[0], GL_STATIC_DRAW);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.

    // set the vertex attribute pointers
    VAO["VBO"].bind();
    // vertex Positions
    VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0);
    // normal ray
    VAO["VBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 8, 3);
    // vertex texture coords
    VAO["VBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 8, 6);

    VAO["VBO"].clear();

    ArrayObject::clear();
}

void Mesh::cleanup() {
    VAO.cleanup();
}
