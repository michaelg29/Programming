#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/shader.h"
#include "rendering/uniformmemory.hpp"
#include "rendering/material.h"
#include "programs/rectangle.hpp"
#include "programs/arrow.hpp"
#include "programs/surface.hpp"
#include "programs/sphere.hpp"

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/camera.h"

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

// IO CALLBACK SIGNATURES ==================================
void processInput(double dt);
void updateCameraMatrices();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods);
void cursorChanged(GLFWwindow* window, double _x, double _y);
void mouseButtonChanged(GLFWwindow* window, int button, int action, int mods);
void scrollChanged(GLFWwindow* window, double dx, double dy);

// GLOBAL VARIABLES =================================
int scr_width = 800, scr_height = 800;
GLFWwindow* window = nullptr;
bool re_render = false;

// camera
Camera cam(glm::vec3(-3.0f, 0.0f, 0.0f));
glm::mat4 view;
glm::mat4 projection;

// programs
Rectangle rect;
Arrow arrow(3);
Surface surface(glm::vec2(-10.0f), glm::vec2(10.0f), 200, 200, -10.0f, 10.0f, Material::yellow_plastic);
Sphere sphere(10);

std::vector<Program*> programs;

typedef struct {
    glm::vec3 direction;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
} DirLight;

int main()
{
    std::cout << "Hello, world!" << std::endl;

    // initialize
    initGLFW(3, 3);
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


    // timing variables
    double dt = 0.0;
    double lastFrame = 0.0;

    // I/O ================
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, Keyboard::keyCallback);
    glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
    glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
    glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
    Keyboard::keyCallbacks.push_back(keyChanged);
    Mouse::cursorPosCallbacks.push_back(cursorChanged);
    Mouse::mouseButtonCallbacks.push_back(mouseButtonChanged);
    Mouse::mouseWheelCallbacks.push_back(scrollChanged);

    // add instances ===============
    // axes
    arrow.addInstance(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 0.01f, 0.02f, 0.1f, Material::cyan_plastic); // x
    arrow.addInstance(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.01f, 0.02f, 0.1f, Material::green_plastic); // y
    arrow.addInstance(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.01f, 0.02f, 0.1f, Material::red_plastic); // z
    // sphere
    sphere.addInstance(glm::vec3(0.0f), glm::vec3(0.05f), Material::white_plastic);

    // register programs =============
    programs.push_back(&rect);
    programs.push_back(&arrow);
    programs.push_back(&surface);
    programs.push_back(&sphere);

    // load programs =============
    for (Program* program : programs)
    {
        program->load();
    }

    // Camera ==============
    framebufferSizeCallback(window, scr_width, scr_height);

    // lighting
    DirLight dirLight = {
        glm::vec3(-0.2f, -0.9f, -0.2f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.75f, 0.75f, 0.75f, 1.0f),
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
    };
    // write to UBO
    UBO::UBO dirLightUBO({
        UBO::newStruct({ // dir light
            UBO::Type::VEC3,
            UBO::Type::VEC4,
            UBO::Type::VEC4,
            UBO::Type::VEC4 
        })
    });
    for (Program* program : programs)
    {
        dirLightUBO.attachToShader(program->shader, "DirLightUniform");
    }
    // generate/bind
    dirLightUBO.generate();
    dirLightUBO.bind();
    dirLightUBO.initNullData(GL_STATIC_DRAW);
    dirLightUBO.bindRange();
    // write
    dirLightUBO.startWrite();
    dirLightUBO.writeElement<glm::vec3>(&dirLight.direction);
    dirLightUBO.writeElement<glm::vec4>(&dirLight.ambient);
    dirLightUBO.writeElement<glm::vec4>(&dirLight.diffuse);
    dirLightUBO.writeElement<glm::vec4>(&dirLight.specular);

    re_render = true;
    
    while (!glfwWindowShouldClose(window)) {
        // update time
        dt = glfwGetTime() - lastFrame;
        lastFrame += dt;
        
        // =================PROCESS INPUT
        // wait for interrupt
        glfwWaitEventsTimeout(0.001);

        processInput(dt);

        // =================UPDATE PROGRAMS
        for (Program* program : programs)
        {
            if (program->update(dt)) {
                re_render = true;
            }
        }

        // =================RENDER
        if (re_render)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // render programs
            for (Program* program : programs)
            {
                program->render();
            }

            glfwSwapBuffers(window);
            re_render = false;
        }
    }

    // =====================CLEANUP
    for (Program* program : programs)
    {
        program->cleanup();
    }

    dirLightUBO.cleanup();

    glfwTerminate();

	return 0;
}

void updateCameraMatrices() {
    view = cam.getViewMatrix();
    projection = glm::perspective(
        glm::radians(cam.getZoom()),	        // FOV
        (float)scr_width / (float)scr_height,	// aspect ratio
        0.1f, 100.0f					        // near and far bounds
    );

    glm::mat4 projView = projection * view;

    // program callbacks
    for (Program* program : programs)
    {
        program->updateCameraMatrices(projView, cam.cameraPos);
    }

    re_render = true;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    scr_width = 800;
    scr_height = 800;
    updateCameraMatrices();
}

void processInput(double dt) {
    // set camera pos/matrices, continuously poll
    if (Keyboard::key(GLFW_KEY_W)) {
        cam.updateCameraPos(CameraDirection::FORWARD, dt);
        updateCameraMatrices();
    }
    if (Keyboard::key(GLFW_KEY_S)) {
        cam.updateCameraPos(CameraDirection::BACKWARD, dt);
        updateCameraMatrices();
    }
    if (Keyboard::key(GLFW_KEY_D)) {
        cam.updateCameraPos(CameraDirection::RIGHT, dt);
        updateCameraMatrices();
    }
    if (Keyboard::key(GLFW_KEY_A)) {
        cam.updateCameraPos(CameraDirection::LEFT, dt);
        updateCameraMatrices();
    }
    if (Keyboard::key(GLFW_KEY_SPACE)) {
        cam.updateCameraPos(CameraDirection::UP, dt);
        updateCameraMatrices();
    }
    if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
        cam.updateCameraPos(CameraDirection::DOWN, dt);
        updateCameraMatrices();
    }
}

void keyChanged(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (Keyboard::key(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    // program callbacks
    for (Program* program : programs)
    {
        program->keyChanged(window, key, scancode, action, mods);
    }
}

void cursorChanged(GLFWwindow* window, double _x, double _y)
{
    // set camera direction
    double dx = Mouse::getDX(), dy = Mouse::getDY();
    if (dx != 0 || dy != 0) {
        cam.updateCameraDirection(dx, dy);
    }

    // set matrices
    updateCameraMatrices();

    // program callbacks
    for (Program* program : programs)
    {
        program->cursorChanged(window, _x, _y);
    }
}

void mouseButtonChanged(GLFWwindow* window, int button, int action, int mods)
{
    // program callbacks
    for (Program* program : programs)
    {
        program->mouseButtonChanged(window, button, action, mods);
    }
}

void scrollChanged(GLFWwindow* window, double dx, double dy)
{
    // set camera zoom
    double scrollDy = Mouse::getScrollDY();
    if (scrollDy != 0) {
        cam.updateCameraZoom(scrollDy);
    }

    // set matrices
    updateCameraMatrices();

    // program callbacks
    for (Program* program : programs)
    {
        program->scrollChanged(window, dx, dy);
    }
}