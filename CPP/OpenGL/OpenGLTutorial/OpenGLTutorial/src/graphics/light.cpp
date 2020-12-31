#include "light.h"

DirLight::DirLight(glm::vec3 direction, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, BoundingRegion shadowRegion)
    : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular),
    shadowFBO(2048, 2048, GL_DEPTH_BUFFER_BIT), br(br) {
    br = BoundingRegion(glm::vec3(-10.0f, -10.0f, 0.5f), glm::vec3(10.0f, 10.0f, 50.0f));
    
    shadowFBO.generate();

    shadowFBO.bind();
    shadowFBO.disableDrawBuffer();
    shadowFBO.disableReadBuffer();
    shadowFBO.allocateAndAttachTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT);

    updateMatrices();
}

// render directional light into shader
void DirLight::render(Shader shader, unsigned int textureIdx) {
    // set direction
    shader.set3Float("dirLight.direction", direction);

    // set lighting values
    shader.set4Float("dirLight.ambient", ambient);
    shader.set4Float("dirLight.diffuse", diffuse);
    shader.set4Float("dirLight.specular", specular);

    // set depth texture
    glActiveTexture(GL_TEXTURE0 + textureIdx);
    shadowFBO.textures[0].bind();
    shader.setInt("dirLight.depthBuffer", textureIdx);

    // set light space matrix
    shader.setMat4("dirLight.lightSpaceMatrix", lightSpaceMatrix);
}

// update light space matrix
void DirLight::updateMatrices() {
    glm::mat4 proj = glm::ortho(br.min.x, br.max.x, br.min.y, br.max.y, br.min.z, br.max.z);

    glm::vec3 pos = -2.0f * direction;

    glm::mat4 lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    lightSpaceMatrix = proj * lightView;
}

// render point light into shader
void PointLight::render(Shader shader, int idx) {
    // get name with index in array
    std::string name = "pointLights[" + std::to_string(idx) + "]";

    // set position
    shader.set3Float(name + ".position", position);

    // set attenuation constants
    shader.setFloat(name + ".k0", k0);
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);

    // set lighting values
    shader.set4Float(name + ".ambient", ambient);
    shader.set4Float(name + ".diffuse", diffuse);
    shader.set4Float(name + ".specular", specular);
}

// constructor
SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 up,
    float cutOff, float outerCutOff,
    float k0, float k1, float k2,
    glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular,
    float nearPlane, float farPlane)
    : position(position), direction(direction), up(up),
    cutOff(cutOff), outerCutOff(outerCutOff),
    k0(k0), k1(k1), k2(k2),
    ambient(ambient), diffuse(diffuse), specular(specular),
    nearPlane(nearPlane), farPlane(farPlane),
    shadowFBO(2048, 2048, GL_DEPTH_BUFFER_BIT) {
    shadowFBO.generate();

    shadowFBO.bind();
    shadowFBO.disableDrawBuffer();
    shadowFBO.disableReadBuffer();
    shadowFBO.allocateAndAttachTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_FLOAT);

    updateMatrices();
}

// render light into shader
void SpotLight::render(Shader shader, int idx, unsigned int textureIdx) {
    // get name with index in array
    std::string name = "spotLights[" + std::to_string(idx) + "]";

    // set position
    shader.set3Float(name + ".position", position);
    // set direction
    shader.set3Float(name + ".direction", direction);

    // set first level cut off
    shader.setFloat(name + ".cutOff", cutOff);
    // set second level cut off
    shader.setFloat(name + ".outerCutOff", outerCutOff);

    // set attenuation constants
    shader.setFloat(name + ".k0", k0);
    shader.setFloat(name + ".k1", k1);
    shader.setFloat(name + ".k2", k2);

    // set lighting values
    shader.set4Float(name + ".ambient", ambient);
    shader.set4Float(name + ".diffuse", diffuse);
    shader.set4Float(name + ".specular", specular);

    // set near and far planes
    shader.setFloat(name + ".nearPlane", nearPlane);
    shader.setFloat(name + ".farPlane", farPlane);

    // set depth texture
    glActiveTexture(GL_TEXTURE0 + textureIdx);
    shadowFBO.textures[0].bind();
    shader.setInt(name + ".depthBuffer", textureIdx);

    // set light space matrix
    shader.setMat4(name + ".lightSpaceMatrix", lightSpaceMatrix);
}

// update light space matrix
void SpotLight::updateMatrices() {
    glm::mat4 proj = glm::perspective(glm::acos(outerCutOff) * 2.0f, // FOV
        (float)shadowFBO.height / (float)shadowFBO.width, // aspect ratio
        nearPlane, farPlane); // near and far bounds

    glm::mat4 lightView = glm::lookAt(position, position + direction, up);

    lightSpaceMatrix = proj * lightView;
}
