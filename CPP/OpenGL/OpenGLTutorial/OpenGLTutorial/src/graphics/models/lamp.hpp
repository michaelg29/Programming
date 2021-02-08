#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../material.h"
#include "../shader.h"
#include "../light.h"

#include <glm/glm.hpp>

class Lamp : public Cube {
public:
    glm::vec3 lightColor;

    Lamp(unsigned int maxNoInstances, glm::vec3 lightColor = glm::vec3(1.0f))
        : Cube(maxNoInstances, Material::white_rubber) {
        id = "lamp";
        this->lightColor = lightColor;
    }

    void render(Shader shader, float dt, Scene *scene, glm::mat4 model = glm::mat4(1.0f)) {
        // set light color
        shader.set3Float("lightColor", lightColor);

        Cube::render(shader, dt, scene, model);
    }
};

#endif