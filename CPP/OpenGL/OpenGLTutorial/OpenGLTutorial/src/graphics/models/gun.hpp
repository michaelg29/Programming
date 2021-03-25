#include "../objects/model.h"

#include "../../io/camera.h"
#include "../../io/keyboard.h"

#include "../../scene.h"

#include "../../physics/environment.h"

#include <iostream>

class Gun : public Model {
public:
    Gun(unsigned int maxNoInstances)
        : Model("m4a1", BoundTypes::AABB, maxNoInstances, NO_TEX | CONST_INSTANCES) {}

    void init() {
        loadModel("assets/models/m4a1/scene.gltf");
    }

    void render(Shader shader, float dt, Scene *scene) {
        //// rotation matrix
        //glm::mat4 rot(1.0f);

        //Camera* cam = scene->getActiveCamera();

        //// set position
        //// multiply offset by unit vector in 2 directions
        //instances[0]->pos = cam->cameraPos + glm::vec3(cam->cameraFront * 0.5f) - glm::vec3(cam->cameraUp * 0.205f);

        //float theta;

        //// rotate around camera right using dot product
        //theta = acos(glm::dot(Environment::worldUp, cam->cameraFront) /
        //    glm::length(cam->cameraUp) / glm::length(cam->cameraFront));
        //rot = glm::rotate(rot, atanf(1) * 2 - theta, cam->cameraRight); // offset by pi/2 radians bc angle btwn camFront and gunFront

        //// rotate around cameraUp using dot product
        //glm::vec2 front2d = glm::vec2(cam->cameraFront.x, cam->cameraFront.z);
        //theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
        //rot = glm::rotate(rot, cam->cameraFront.z < 0 ? theta : -theta, Environment::worldUp);

        //glm::vec3 rotatedFront = glm::vec3(rot * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        //glm::quat q = glm::quatLookAt(rotatedFront, cam->cameraUp);
        //
        //// apply rotation
        //instances[0]->rot = glm::eulerAngles(q);
        
        Model::render(shader, dt, scene);
    }
};