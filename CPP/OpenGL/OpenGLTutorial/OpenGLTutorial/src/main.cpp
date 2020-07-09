#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <stack>

#include <glm/glm.hpp>

#include "scene.h"

#include "io/keyboard.h"
#include "io/mouse.h"
#include "io/joystick.h"
#include "io/camera.h"

#include "graphics/shader.h"
#include "graphics/light.h"

#include "graphics/models/gun.hpp"
#include "graphics/models/cube.hpp"
#include "graphics/models/lamp.hpp"
#include "graphics/models/sphere.hpp"
#include "graphics/models/box.hpp"
#include "graphics/models/troll.hpp"

#include "physics/environment.h"
#include "physics/rigidbody.h"

#include "algorithms/states.hpp"

void processInput(double dt);

//Joystick mainJ(0);
Camera cam(glm::vec3(0.0f));
Scene scene;

Sphere sphere(10);

double dt = 0.0f; // time btwn frames
double lastFrame = 0.0f; // time of last frame

glm::vec3 Environment::gravity = glm::vec3(0.0f, -9.81f, 0.0f);

int main() {
	std::cout << "Hello, OpenGL!" << std::endl;

	scene = Scene(3, 3, "OpenGL Tutorial", 800, 600);

	if (!scene.init()) {
		std::cout << "Could not open window" << std::endl;
		scene.cleanup();
		return -1;
	}

	// shader
	Shader shader("assets/instanced.vs", "assets/object.fs");
	Shader lightShader("assets/instanced.vs", "assets/lamp.fs");
	Shader boxShader("assets/box.vs", "assets/box.fs");

	// camera
	scene.cameras.push_back(&cam);
	scene.activeCamera = 0;

	// objects
	Lamp lamp(4);
	scene.registerModel(&lamp);

	scene.registerModel(&sphere);

	//Troll troll(1);
 	//scene.registerModel(&troll);

	Box box;
	box.init();

	// load all model data
	scene.loadModels();

	// lighting
	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f) };
	scene.dirLight = &dirLight;

	glm::vec4 ambient = glm::vec4(glm::vec3(0.05f), 1.0f);
	glm::vec4 diffuse = glm::vec4(glm::vec3(0.8f), 1.0f);
	glm::vec4 specular = glm::vec4(glm::vec3(1.0f), 1.0f);
	float k0 = 1.0f;
	float k1 = 0.09f;
	float k2 = 0.032f;

	PointLight pointLights[] = {
		{ glm::vec3(0.7f,  0.2f,  2.0f), k0, k1, k2, ambient, diffuse, specular },
		{ glm::vec3(2.3f, -3.3f, -4.0f), k0, k1, k2, ambient, diffuse, specular },
		{ glm::vec3(-4.0f,  2.0f, -12.0f), k0, k1, k2, ambient, diffuse, specular },
		{ glm::vec3(0.0f,  0.0f, -3.0f), k0, k1, k2, ambient, diffuse, specular }
	};

	for (unsigned int i = 0; i < 4; i++) {
		scene.generateInstance("lamp", glm::vec3(0.25f), 0.25f, pointLights[i].position);
		scene.pointLights.push_back(&pointLights[i]);
		States::activateIndex(&scene.activePointLights, i);
	}

	SpotLight spotLight = { 
		cam.cameraPos, cam.cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)),
		1.0f, 0.09f, 0.032f, 
		glm::vec4(0.0f), glm::vec4(1.0f), glm::vec4(1.0f) 
	};
	scene.spotLights.push_back(&spotLight);
	scene.activeSpotLights = 1;

	// add static instances
	//scene.generateInstance("troll", glm::vec3(0.025f), 100.0f, glm::vec3(1.0f));

	// instantiate instances
	scene.initInstances();

	// prepare scene
	scene.prepare(box);

	/*mainJ.update();
	if (mainJ.isPresent()) {
		std::cout << mainJ.getName() << " is present." << std::endl;
	}*/

	while (!scene.shouldClose()) {
		box.positions.clear();
		box.sizes.clear();

		// calculate dt
		double currentTime = glfwGetTime();
		dt = currentTime - lastFrame;
		lastFrame = currentTime;
		
		scene.update(box);

		// process input
		processInput(dt);

		// render troll
		//scene.renderShader(shader);
		//scene.renderInstances("troll", shader, dt);

		// remove launch objects if too far
		for (int i = 0; i < sphere.currentNoInstances; i++) {
			if (glm::length(cam.cameraPos - sphere.instances[i]->pos) > 50.0f) {
				scene.markForDeletion(sphere.instances[i]->instanceId);
			}
		}

		// render launch objects
		if (sphere.currentNoInstances > 0) {
			scene.renderShader(shader);
			scene.renderInstances("sphere", shader, dt);
		}

		// render lamps
		scene.renderShader(lightShader, false);
		scene.renderInstances("lamp", lightShader, dt);

		// render box
		scene.renderShader(boxShader, false);
		box.render(boxShader);

		scene.clearDeadInstances();
		scene.newFrame();
	}

	scene.cleanup();
	return 0;
}

void launchItem(float dt) {
	RigidBody* rb = scene.generateInstance("sphere", glm::vec3(1.0f), 1.0f, cam.cameraPos);
	if (rb) {
		rb->transferEnergy(100.0f, cam.cameraFront);
		rb->applyAcceleration(Environment::gravity);
	}
}

void processInput(double dt) {
	scene.processInput(dt);

	// update flash light
	if (States::isActive<unsigned int>(&scene.activeSpotLights, 0)) {
		scene.spotLights[0]->position = scene.getActiveCamera()->cameraPos;
		scene.spotLights[0]->direction = scene.getActiveCamera()->cameraFront;
	}

	if (Keyboard::key(GLFW_KEY_ESCAPE)) {
		scene.setShouldClose(true);
	}

	if (Keyboard::keyWentDown(GLFW_KEY_L)) {
		States::toggleIndex(&scene.activeSpotLights, 0); // toggle spot light
	}

	for (int i = 0; i < scene.pointLights.size(); i++) {
		if (Keyboard::keyWentDown(GLFW_KEY_1 + i)) {
			States::toggleIndex(&scene.activePointLights, i);
		}
	}

	// launch item
	if (Keyboard::keyWentDown(GLFW_KEY_F)) {
		launchItem(dt);
	}
}