#ifndef SCENE_H
#define SCENE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>
#include <string>

#include <jsoncpp/json.hpp>

#include <glm/glm.hpp>

#include "graphics/light.h"
#include "graphics/shader.h"
#include "graphics/model.h"

#include "physics/rigidbody.h"

#include "io/camera.h"
#include "io/keyboard.h"
#include "io/mouse.h"

#include "algorithms/trie.hpp"
#include "algorithms/octree.h"

class Scene {
public:
	// trie for models
	trie::Trie<Model*> models;

	// trie for instances
	trie::Trie<RigidBody*> instances;

	// list of instances to delete
	std::vector<RigidBody*> instancesToDelete;

	// octree object
	Octree::node* octree;

	// map for logged variables
	jsoncpp::json variableLog;

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

	// prepare for main loop (octree, etc)
	void prepare(Box &box);

	/*
		main loop methods
	*/
	// process input
	void processInput(float dt);

	// update screen before each frame
	void update();

	// update screen after each frame
	void newFrame(Box& box);

	// set uniform shader variables (lighting, etc)
	void renderShader(Shader shader, bool applyLighting = true);

	// render instances
	void renderInstances(std::vector<std::string> modelIds, Shader shader, float dt);
	void renderInstances(std::string modelId, Shader shader, float dt);

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
		Model/instance methods
	*/
	std::string currentId = "aaaaaaaa";

	std::string generateId();

	void registerModel(Model* model);

	RigidBody* generateInstance(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos);

	void initInstances();

	void loadModels();

	void removeInstance(std::string instanceId);

	void markForDeletion(std::string instanceId);

	void clearDeadInstances();

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