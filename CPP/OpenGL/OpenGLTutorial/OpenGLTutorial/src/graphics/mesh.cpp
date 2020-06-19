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

// default constructor
Mesh::Mesh() {}

// initialize as textured object
Mesh::Mesh(BoundingRegion br, std::vector<Texture> textures)
    : br(br), textures(textures), noTex(false) {}

// initialize as material object
Mesh::Mesh(BoundingRegion br, aiColor4D diff, aiColor4D spec)
    : br(br), diff(diff), spec(spec), noTex(true) {}

// load vertex and index data
void Mesh::loadData(Vertex* vertices, unsigned int noVertices, unsigned int* indices, unsigned int noIndices) {
    // bind VAO
    VAO.generate();
    VAO.bind();

    // generate/set EBO
    VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
    VAO["EBO"].generate();
    VAO["EBO"].bind();
    VAO["EBO"].setData<GLuint>(noIndices, indices, GL_STATIC_DRAW);

    // load data into vertex buffers
    VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
    VAO["VBO"].generate();
    VAO["VBO"].bind();
    VAO["VBO"].setData<Vertex>(noVertices, vertices, GL_STATIC_DRAW);

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

// render number of instances
void Mesh::render(Shader shader, unsigned int noInstances) {
    if (noTex) {
        // set material vals
        shader.set4Float("material.diffuse", diff);
        shader.set4Float("material.specular", spec);
        shader.setInt("noTex", 1);
    }
    else {
        // set textures
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

    VAO.bind();
    VAO.draw(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, noInstances);
    ArrayObject::clear();

    glActiveTexture(GL_TEXTURE0);
}

// cleanup (free vectors, cleanup VAO)
void Mesh::cleanup() {
    VAO.cleanup();
}
