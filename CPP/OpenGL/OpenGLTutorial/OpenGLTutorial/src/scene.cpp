#include "scene.h"

#include "algorithms/states.hpp"
#include "algorithms/list.hpp"

unsigned int Scene::scrWidth = 0;
unsigned int Scene::scrHeight = 0;

/*
	Callbacks
*/

// window resize
void Scene::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	scrWidth = width;
	scrHeight = height;
}

/*
	Constructor
*/
Scene::Scene(int glfwVersionMajor, int glfwVersionMinor,
			 const char *title, unsigned int _scrWidth, unsigned int _scrHeight)
	: glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor),
	  title(title),
	  activeCamera(-1)
{
	scrWidth = _scrWidth;
	scrHeight = _scrHeight;

	setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

bool Scene::init()
{
	glfwInit();

	// set version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glfwVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glfwVersionMinor);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// initialize window
	window = glfwCreateWindow(scrWidth, scrHeight, title, NULL, NULL);
	if (window == NULL) {
		// not created
		return false;
	}
	glfwMakeContextCurrent(window);

	// set GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// setup screen
	glViewport(0, 0, scrWidth, scrHeight);

	/*
		callbacks
	*/
	// frame size
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	// key pressed
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	// cursor moved
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	// mouse btn pressed
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	// mouse scroll
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	/*
		set rendering parameters
	*/
	// so doesn't show vertices not visible to camera (ie back of object)
	glEnable(GL_DEPTH_TEST);
	// disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return true;
}

/*
	main loop methods
*/
// process input
void Scene::processInput(float dt) {
	if (activeCamera != -1 && activeCamera < cameras.size()) {
		// active camera exists

		// set camera direction
		double dx = Mouse::getDX(),
			dy = Mouse::getDY();;
		if (dx != 0 || dy != 0) {
			cameras[activeCamera]->updateCameraDirection(dx, dy);
		}

		// set camera zoom
		double scrollDy = Mouse::getScrollDY();
		if (scrollDy != 0) {
			cameras[activeCamera]->updateCameraZoom(scrollDy);
		}

		// set camera pos
		if (Keyboard::key(GLFW_KEY_W)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::FORWARD, dt);
		}
		if (Keyboard::key(GLFW_KEY_S)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::BACKWARD, dt);
		}
		if (Keyboard::key(GLFW_KEY_D)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::RIGHT, dt);
		}
		if (Keyboard::key(GLFW_KEY_A)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::LEFT, dt);
		}
		if (Keyboard::key(GLFW_KEY_SPACE)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::UP, dt);
		}
		if (Keyboard::key(GLFW_KEY_LEFT_SHIFT)) {
			cameras[activeCamera]->updateCameraPos(CameraDirection::DOWN, dt);
		}

		// set matrices
		view = cameras[activeCamera]->getViewMatrix();
		projection = glm::perspective(
			glm::radians(cameras[activeCamera]->getZoom()), // FOV
			(float)scrWidth / (float)scrHeight,				// aspect ratio
			0.1f, 100.0f									// near and far bounds
		);

		// set pos at end
		cameraPos = cameras[activeCamera]->cameraPos;
	}
}

// update screen before each frame
void Scene::update()
{
	glClearColor(bg[0], bg[1], bg[2], bg[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// update screen after each frame
void Scene::newFrame() {
	// send new frame to window
	glfwSwapBuffers(window);
	glfwPollEvents();
}

// set uniform shader variables (lighting, etc)
void Scene::renderShader(Shader shader, bool applyLighting) {
	shader.activate();

	// set camera values
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.set3Float("viewPos", cameraPos);

	if (applyLighting) {
		/*
			lighting
		*/

		// point lights
		unsigned int noLights = pointLights.size();
		unsigned int noActiveLights = 0;
		for (unsigned int i = 0; i < noLights; i++) {
			if ((activePointLights & (1 << i)) == (1 << i)) {
				pointLights[i]->render(shader, noActiveLights);
				noActiveLights++;
			}
		}
		shader.setInt("noPointLights", noActiveLights);

		// spot lights
		noLights = spotLights.size();
		noActiveLights = 0;
		for (unsigned int i = 0; i < noLights; i++) {
			if ((activeSpotLights & (1 << i)) == (1 << i)) {
				spotLights[i]->render(shader, noActiveLights);
				noActiveLights++;
			}
		}
		shader.setInt("noSpotLights", noActiveLights);

		dirLight->render(shader);
	}
}

// render instances
void Scene::renderInstances(std::vector<std::string> modelIds, Shader shader, float dt) {
	for (std::string id : modelIds) {
		if (models[id]->currentNoInstances > 0) {
			models[id]->render(shader, dt, this);
		}
	}
}

void Scene::renderInstances(std::string modelId, Shader shader, float dt) {
	models[modelId]->render(shader, dt, this);
}

void Scene::cleanup() {
	models.traverse([](Model* model) -> void {
		model->cleanup();
	});

	glfwTerminate();
}

/*
	Accesors
*/

bool Scene::shouldClose() {
	return glfwWindowShouldClose(window);
}

Camera *Scene::getActiveCamera() {
	return activeCamera == -1 ? nullptr : cameras[activeCamera];
}

/*
	Modifiers
*/

void Scene::setShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(window, shouldClose);
}

void Scene::setWindowColor(float r, float g, float b, float a) {
	bg[0] = r;
	bg[1] = g;
	bg[2] = b;
	bg[3] = a;
}

/*
	Model/instance methods
*/
std::string Scene::generateId() {
	for (int i = currentId.length() - 1; i >= 0; i--) {
		if ((int)currentId[i] != (int)'z') {
			currentId[i] = (char)(((int)currentId[i]) + 1);
			break;
		}
		else {
			currentId[i] = 'a';
		}
	}

	return currentId;
}

void Scene::registerModel(Model* model) {
	models.insert(model->id, model);

	//models[model->id] = model;
}

RigidBody* Scene::generateInstance(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos) {
	RigidBody* rb = models[modelId]->generateInstance(size, mass, pos);
	if (rb) {
		std::string id = generateId();
		rb->instanceId = id;
		instances.insert(id, rb);
		//instances[id] = { modelId, idx };
		return rb;
	}
	return nullptr;
}

void Scene::initInstances() {
	models.traverse([](Model* model) -> void {
		model->initInstances();
	});
	/*for (auto& pair : models) {
		pair.second->initInstances();
	}*/
}

void Scene::loadModels() {
	models.traverse([](Model* model) -> void {
		model->init();
	});
	/*for (auto& pair : models) {
		pair.second->init();
	}*/
}

void Scene::removeInstance(std::string instanceId) {
	/*
		remove all locations
		- Scene::instances
		- Model::instances
	*/

	std::string targetModel = instances[instanceId]->modelId;
	//unsigned int targetIdx = instances[instanceId].second;

	models[targetModel]->removeInstance(instanceId);

	instances.erase(instanceId);
}

void Scene::markForDeletion(std::string instanceId) {
	States::activate(&instances[instanceId]->state, INSTANCE_DEAD);
	instancesToDelete.push_back(instances[instanceId]);
}

void Scene::clearDeadInstances() {
	for (RigidBody* rb : instancesToDelete) {
		removeInstance(rb->instanceId);
	}
	instancesToDelete.clear();
}