#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../rendering/shader.h"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
public:
	Shader shader;

	virtual void load();

	void updateCameraMatrices(glm::mat4 projView, glm::vec3 camPos);
	virtual void processInput(double dt, GLFWwindow* window);
	virtual void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
	virtual void cursorChanged(GLFWwindow* window, double _x, double _y);
	virtual void mouseButtonChanged(GLFWwindow* window, int button, int action, int mods);
	virtual void scrollChanged(GLFWwindow* window, double dx, double dy);

	virtual void render(double dt);

	virtual void cleanup();
};

#endif