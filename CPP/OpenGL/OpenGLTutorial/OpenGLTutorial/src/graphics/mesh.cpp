#include "mesh.h"

#include <iostream>

// generate list of vertices
std::vector<Vertex> Vertex::genList(float* vertices, int noVertices) {
    std::vector<Vertex> ret(noVertices);

    int stride = 8;

    for (int i = 0; i < noVertices; i++) {
        ret[i].pos = glm::vec3(
            vertices[i * stride + 0],
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

        ret[i].tangent = glm::vec3(0.0f);
        ret[i].bitangent = glm::vec3(0.0f);
    }

    return ret;
}

// calculate tangent and bitangent vectors for each face
void Vertex::calcTanBiTanVectors(std::vector<Vertex>& list, std::vector<unsigned int>& indices) {
    // iterate through indices and calculate vectors for each face
    for (unsigned int i = 0, len = indices.size(); i < len; i += 3) {
        Vertex v1 = list[indices[i + 0]];
        Vertex v2 = list[indices[i + 1]];
        Vertex v3 = list[indices[i + 2]];

        // calculate edges
        glm::vec3 edge1 = v2.pos - v1.pos;
        glm::vec3 edge2 = v2.pos - v1.pos;

        // calculate UVs
        glm::vec2 deltaUV1 = v2.texCoord - v1.texCoord;
        glm::vec2 deltaUV2 = v3.texCoord - v1.texCoord;

        // use inverse of matrix to determine tangent and bitangent
        /*
            | E_1x E_1y E_1z |   | dU_1 dV_1 | * | T_x T_y T_z |
            | E_2x E_2y E_2z | = | dU_2 dV_2 |   | B_x B_y B_z |
        */
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent = {
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        };

        glm::vec3 bitangent = {
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        };

        list[indices[i + 0]].tangent = tangent;
        list[indices[i + 1]].tangent = tangent;
        list[indices[i + 2]].tangent = tangent;
        
        list[indices[i + 0]].bitangent = bitangent;
        list[indices[i + 1]].bitangent = bitangent;
        list[indices[i + 2]].bitangent = bitangent;
    }
}

/*
    constructors
*/

// default
Mesh::Mesh() {}

// initialize as textured object
Mesh::Mesh(BoundingRegion br, std::vector<Texture> textures)
    : br(br), textures(textures), noTex(false) {}

// initialize as material object
Mesh::Mesh(BoundingRegion br, aiColor4D diff, aiColor4D spec)
    : br(br), diffuse(diff), specular(spec), noTex(true) {}

// load vertex and index data
void Mesh::loadData(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices, bool pad) {
    this->vertices = _vertices;
    this->indices = _indices;

    // bind VAO
    VAO.generate();
    VAO.bind();

    // generate/set EBO
    VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
    VAO["EBO"].generate();
    VAO["EBO"].bind();
    VAO["EBO"].setData<GLuint>(this->indices.size(), &this->indices[0], GL_STATIC_DRAW);

    // load data into vertex buffers
    VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
    VAO["VBO"].generate();
    VAO["VBO"].bind();

    unsigned int size = this->vertices.size();
    if (pad && size) {
        size++;
    }

    VAO["VBO"].setData<Vertex>(size, &this->vertices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    VAO["VBO"].bind();
    // vertex Positions
    VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 14, 0);
    // normal ray
    VAO["VBO"].setAttPointer<GLfloat>(1, 3, GL_FLOAT, 14, 3);
    // vertex texture coords
    VAO["VBO"].setAttPointer<GLfloat>(2, 2, GL_FLOAT, 14, 6);
    // tangent vectors
    VAO["VBO"].setAttPointer<GLfloat>(3, 3, GL_FLOAT, 14, 8);
    // bitangent vectors
    VAO["VBO"].setAttPointer<GLfloat>(4, 3, GL_FLOAT, 14, 11);

    VAO["VBO"].clear();

    ArrayObject::clear();
}

// render number of instances using shader
void Mesh::render(Shader shader, unsigned int noInstances) {
    shader.setBool("noNormal", true);

    if (noTex) {
        // materials
        shader.set4Float("material.diffuse", diffuse);
        shader.set4Float("material.specular", specular);
        shader.setBool("noTex", true);
    }
    else {
        // textures
        unsigned int diffuseIdx = 0;
        unsigned int normalIdx = 0;
        unsigned int specularIdx = 0;

        for (unsigned int i = 0; i < textures.size(); i++) {
            // activate texture
            glActiveTexture(GL_TEXTURE0 + i);

            // retrieve texture info
            std::string name;
            switch (textures[i].type) {
            case aiTextureType_DIFFUSE:
                name = "diffuse" + std::to_string(diffuseIdx++);
                break;
            case aiTextureType_NORMALS:
                name = "normal" + std::to_string(normalIdx++);
                shader.setBool("noNormal", false);
                break;
            case aiTextureType_SPECULAR:
                name = "specular" + std::to_string(specularIdx++);
                break;

            default:
                name = textures[i].name;
                break;
            }

            // set the shader value
            shader.setInt(name, i);
            // bind texture
            textures[i].bind();
        }
        shader.setBool("noTex", false);
    }
    
    VAO.bind();
    VAO.draw(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, noInstances);
    ArrayObject::clear();

    glActiveTexture(GL_TEXTURE0);
}

// free up memory
void Mesh::cleanup() {
    VAO.cleanup();

    for (Texture t : textures) {
        t.cleanup();
    }
}