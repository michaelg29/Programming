#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/glm.hpp>

#include "graphics/light.h"
#include "graphics/shader.h"

#include "io/camera.h"
#include "io/keyboard.h"
#include "io/mouse.h"

class Scene {
public:
	/*
		Callbacks
	*/
	// window resize
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	/*
		Constructor
	*/
	Scene() {}
	Scene(int glfwVersionMajor, int glfwVersionMinor,
		const char* title, unsigned int scWidth, unsigned int scrHeight);

	/*
		initialization method (parameters, etc)
	*/
	bool init();

	/*
		main loop methods
	*/
	// process input
	void processInput(float dt);

	// update screen before each frame
	void update();

	// update screen after each frame
	void newFrame();

	// set uniform shader variables (lighting, etc)
	void render(Shader shader, bool applyLighting);

	/*
		cleanup method
	*/
	void cleanup();

	/*
		Accessors
	*/
	bool shouldClose();

	Camera* getActiveCamera();

	/*
		Modifiers
	*/
	void setShouldClose(bool shouldClose);

	void setWindowColor(float r, float g, float b, float a);

	/*
		Lights
	*/
	// list of point lights
	std::vector<PointLight*> pointLights;
	unsigned int activePointLights;
	// list of spot lights
	std::vector<SpotLight*> spotLights;
	unsigned int activeSpotLights;
	// direction light
	DirLight* dirLight;
	bool dirLightActive;

	/*
		Camera
	*/
	std::vector<Camera*> cameras;
	unsigned int activeCamera;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 cameraPos;

protected:
	// window object
	GLFWwindow* window;

	// window values
	const char* title;
	static unsigned int scrWidth;
	static unsigned int scrHeight;
	float bg[4];

	// GLFW info
	int glfwVersionMajor;
	int glfwVersionMinor;

};

#endif