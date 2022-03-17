#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "programs/shader.h"
#include "programs/2d/grid2d.hpp"

std::string Shader::defaultDirectory = "assets/shaders";

// initialize GLFW
void initGLFW(unsigned int versionMajor, unsigned int versionMinor) {
    // initialize context
    glfwInit();

    // pass in parameters
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // macos specific parameter
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void createWindow(GLFWwindow*& window,
    const char* title, unsigned int width, unsigned int height,
    GLFWframebuffersizefun framebufferSizeCallback) {
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main()
{
	std::cout << "Hello, world!" << std::endl;

    initGLFW(3, 3);

    GLFWwindow* window = nullptr;
    createWindow(window, "GLMath", 800, 800, framebufferSizeCallback);

    if (!window) {
        std::cout << "Could not create window" << std::endl;
        return -1;
    }

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Could not load GLAD" << std::endl;
        return -1;
    }

    // set viewport
    framebufferSizeCallback(window, 800, 800);

    // timing variables
    double dt = 0.0;
    double lastFrame = 0.0;

    // programs

    Grid2D grid({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, 5, 5);
    grid.load();

    while (!glfwWindowShouldClose(window)) {
        // update time
        dt = glfwGetTime() - lastFrame;
        lastFrame += dt;
        
        glfwPollEvents();

        // =================PROCESS INPUT
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
        grid.processInput(dt, window);

        // =================RENDER
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render programs
        grid.render(dt);

        glfwSwapBuffers(window);
    }

    // =====================CLEANUp
    grid.cleanup();

    glfwTerminate();

	return 0;
}