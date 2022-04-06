#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "programs/shader.h"
#include "programs/3d/arrow.hpp"

#include "io/keyboard.h"
#include "io/mouse.h"

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

void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorChanged(GLFWwindow* window, double _x, double _y);
void mouseButtonChanged(GLFWwindow* window, int button, int action, int mods);
void scrollChanged(GLFWwindow* window, double dx, double dy);

Arrow a(3);

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

    // depth testing
    glEnable(GL_DEPTH_TEST);
    // v-sync
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // disable cursor

    // set viewport
    framebufferSizeCallback(window, 800, 800);

    // timing variables
    double dt = 0.0;
    double lastFrame = 0.0;

    // I/O
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
    Keyboard::keyCallbacks.push_back(keyChanged);
    Mouse::cursorPosCallbacks.push_back(cursorChanged);
    Mouse::mouseButtonCallbacks.push_back(mouseButtonChanged);
    Mouse::mouseWheelCallbacks.push_back(scrollChanged);

    // programs
    a.addInstance(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.01f, 0.02f, 0.1f, glm::vec3(0.0f, 0.0f, 1.0f));
    a.addInstance(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.01f, 0.02f, 0.1f, glm::vec3(1.0f, 0.0f, 0.0f));
    a.addInstance(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.01f, 0.02f, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    a.load();

    while (!glfwWindowShouldClose(window)) {
        // update time
        dt = glfwGetTime() - lastFrame;
        lastFrame += dt;
        
        glfwWaitEventsTimeout(0.001);

        // =================PROCESS INPUT
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
        a.processInput(dt, window);

        // =================RENDER
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render programs
        a.render(dt);

        glfwSwapBuffers(window);
    }

    // =====================CLEANUP
    a.cleanup();

    glfwTerminate();

	return 0;
}

void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    a.keyChanged(window, key, scancode, action, mods);
}

void cursorChanged(GLFWwindow* window, double _x, double _y)
{
    a.cursorChanged(window, _x, _y);
}

void mouseButtonChanged(GLFWwindow* window, int button, int action, int mods)
{
    a.mouseButtonChanged(window, button, action, mods);
}

void scrollChanged(GLFWwindow* window, double dx, double dy)
{
    a.scrollChanged(window, dx, dy);
}