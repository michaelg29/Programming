#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
public:
	virtual void load();

	virtual void processInput(double dt, GLFWwindow* window);
	virtual void render(double dt);

	virtual void cleanup();
};

#endif