#include "octreerender.h"

void OctreeRender::render(Shader geoShader, Octree::node* root) {
    geoShader.activate();
    geoShader.set3Float("center", glm::vec3(0.5f));
    geoShader.set3Float("max", glm::vec3(0.25f));
    geoShader.set3Float("min", glm::vec3(-0.25f));

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 1);
}

void OctreeRender::setup() {
    float points[] = { 0.0f, 0.0f, 0.0f };

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glBindVertexArray(0);
}

void OctreeRender::cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VBO);
}