#include "program.h"

void Program::load() {}

void Program::updateCameraMatrices(glm::mat4 view, glm::mat4 projection, glm::vec3 camPos) {
	shader.activate();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.set3Float("viewPos", camPos);
}
void Program::processInput(double dt, GLFWwindow* window) {}
void Program::keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods) {}
void Program::cursorChanged(GLFWwindow* window, double _x, double _y) {}
void Program::mouseButtonChanged(GLFWwindow* window, int button, int action, int mods) {}
void Program::scrollChanged(GLFWwindow* window, double dx, double dy) {}
void Program::render(double dt) {}
void Program::cleanup() {}