#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

#include "program.h"
#include "../io/keyboard.h"
#include "../rendering/shader.h"
#include "../rendering/material.h"
#include "../rendering/vertexmemory.hpp"
#include "../rendering/uniformmemory.hpp"
#include "../rendering/transition.hpp"
#include "../rendering/vertex.hpp"

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

class Cylinder : public Program {
    unsigned int maxNoInstances;
    unsigned int noInstances;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::vector<glm::vec3> offsets;
    std::vector<glm::vec3> sizes;
    std::vector<glm::vec3> diffuse;
    std::vector<glm::vec4> specular;

    ArrayObject VAO;

public:
    Cylinder(unsigned int maxNoInstances)
        : maxNoInstances(maxNoInstances), noInstances(0) {}

    void load() {
        shader = Shader(false, "3d/cylinder.vs", "3d/dirlight.fs");

        // generate vertices
        unsigned int res = 15;

        // top and bottom
        float th = 0.0f;
        float angleStep = glm::two_pi<float>() / (float)res;
        float texX = 0.0f;
        float texStep = 1 / (float)res;
        for (unsigned int i = 0; i < res; i++, th += angleStep, texX += texStep)
        {
            float x = glm::cos(th);
            float z = glm::sin(th);

            // bottom
            vertices.push_back({
                { x, 0.0f, z },
                { texX, 1.0f },
                { x, 0.0f, z }
                });

            // top
            vertices.push_back({
                { x, 1.0f, z },
                { texX, 0.0f },
                { x, 0.0f, z }
                });

            // indices
            unsigned int nextIdx = (i + 1) % res;

            indices.push_back(i * 2); // bottom left
            indices.push_back(i * 2 + 1); // top left
            indices.push_back(nextIdx * 2 + 1); // top right

            indices.push_back(nextIdx * 2 + 1); // top right
            indices.push_back(nextIdx * 2); // bottom right
            indices.push_back(i * 2); // bottom left
        }

        // setup VAO
        VAO.generate();
        VAO.bind();

        // load data into vertex buffers
        VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
        VAO["VBO"].generate();
        VAO["VBO"].bind();
        VAO["VBO"].setData<Vertex>((GLuint)vertices.size(), &vertices[0], GL_STATIC_DRAW);
        VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 8, 0); // position
        VAO["VBO"].setAttPointer<GLfloat>(1, 2, GL_FLOAT, 8, 3); // texCoord
        VAO["VBO"].setAttPointer<GLfloat>(2, 3, GL_FLOAT, 8, 5); // normal

        VAO["EBO"] = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
        VAO["EBO"].generate();
        VAO["EBO"].bind();
        VAO["EBO"].setData<GLuint>((GLuint)indices.size(), &indices[0], GL_STATIC_DRAW);

        if (noInstances > 0)
        {
            VAO["offsetVBO"] = BufferObject(GL_ARRAY_BUFFER);
            VAO["offsetVBO"].generate();
            VAO["offsetVBO"].bind();
            VAO["offsetVBO"].setData<glm::vec3>(noInstances, &offsets[0], GL_DYNAMIC_DRAW);
            VAO["offsetVBO"].setAttPointer<GLfloat>(3, 3, GL_FLOAT, 3, 0, 1);

            VAO["sizeVBO"] = BufferObject(GL_ARRAY_BUFFER);
            VAO["sizeVBO"].generate();
            VAO["sizeVBO"].bind();
            VAO["sizeVBO"].setData<glm::vec3>(noInstances, &sizes[0], GL_STATIC_DRAW);
            VAO["sizeVBO"].setAttPointer<GLfloat>(4, 3, GL_FLOAT, 3, 0, 1);

            VAO["diffVBO"] = BufferObject(GL_ARRAY_BUFFER);
            VAO["diffVBO"].generate();
            VAO["diffVBO"].bind();
            VAO["diffVBO"].setData<glm::vec3>(noInstances, &diffuse[0], GL_STATIC_DRAW);
            VAO["diffVBO"].setAttPointer<GLfloat>(5, 3, GL_FLOAT, 3, 0, 1);

            VAO["specularVBO"] = BufferObject(GL_ARRAY_BUFFER);
            VAO["specularVBO"].generate();
            VAO["specularVBO"].bind();
            VAO["specularVBO"].setData<glm::vec4>(noInstances, &specular[0], GL_STATIC_DRAW);
            VAO["specularVBO"].setAttPointer<GLfloat>(6, 4, GL_FLOAT, 4, 0, 1);
        }
    }

    bool addInstance(glm::vec3 offset, glm::vec3 size, Material material) {
        if (noInstances >= maxNoInstances)
        {
            return false;
        }

        offsets.push_back(offset);
        sizes.push_back(size);
        diffuse.push_back(material.diffuse);
        specular.push_back(glm::vec4(material.specular, material.shininess));

        noInstances++;

        return true;
    }

    void render() {
        shader.activate();
        VAO.bind();
        VAO.draw(GL_TRIANGLES, (GLuint)indices.size(), GL_UNSIGNED_INT, 0, noInstances);
    }

    void cleanup() {
        noInstances = 0;

        offsets.clear();
        sizes.clear();
        diffuse.clear();
        specular.clear();

        shader.cleanup();
        VAO.cleanup();
    }
};

#endif // CYLINDER_HPP