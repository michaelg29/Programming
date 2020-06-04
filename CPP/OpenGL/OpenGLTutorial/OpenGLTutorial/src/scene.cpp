#include "scene.h"
#include <iostream>

unsigned int Scene::scrWidth = 0;
unsigned int Scene::scrHeight = 0;

/*
	Callbacks
*/

// window resize
void Scene::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	scrWidth = width;
	scrHeight = height;
}

/*
	Constructor
*/
Scene::Scene(int glfwVersionMajor, int glfwVersionMinor,
	const char* title, unsigned int _scrWidth, unsigned int _scrHeight)
	: glfwVersionMajor(glfwVersionMajor), glfwVersionMinor(glfwVersionMinor),
	title(title),
	activeCamera(-1) 
{
	scrWidth = _scrWidth;
	scrHeight = _scrHeight;

	setWindowColor(0.1f, 0.15f, 0.15f, 1.0f);
}

bool Scene::init() {
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
	if (window == NULL) { // not created
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
		cameras[activeCamera]->updateCameraDirection(Mouse::getDX(), Mouse::getDY());

		// set camera zoom
		cameras[activeCamera]->updateCameraZoom(Mouse::getScrollDY());

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
			glm::radians(cameras[activeCamera]->getZoom()),	// FOV
			(float)scrWidth / (float)scrHeight,				// aspect ratio
			0.1f, 100.0f									// near and far bounds
		);

		// set pos at end
		cameraPos = cameras[activeCamera]->cameraPos;

		// update spot lights
		for (unsigned int i = 0, noLights = spotLights.size(); i < noLights; i++) {
			if ((activeSpotLights & (1 << i)) == (1 << i)) {
				spotLights[i]->position = cameras[activeCamera]->cameraPos;
				spotLights[i]->direction = cameras[activeCamera]->cameraFront;
			}
		}
	}
}

// update screen before each frame
void Scene::update() {
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
void Scene::render(Shader shader) {
	shader.activate();

	// set camera values
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.set3Float("viewPos", cameraPos);

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

void Scene::cleanup() {
	glfwTerminate();
}

/*
	Accesors
*/

bool Scene::shouldClose() {
	return glfwWindowShouldClose(window);
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