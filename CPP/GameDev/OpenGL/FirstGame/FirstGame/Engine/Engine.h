#ifndef ENGINE
#define ENGINE

#include "IO/Joystick.h"

#include "GLFW/glfw3.h"
#pragma comment(lib, "opengl32.lib")

class Engine {
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static float GetDT();

	Engine();
	~Engine();

	bool Initialize(const char* windowTitle);
	void UseJoysticks();

	void Update();
	void BeginRender();
	void EndRender();

	Joystick GetJoystick(int id);

private:
	static GLFWwindow* window;

	static float dt;
	float lastTime;
	bool joystickUse;

	Joystick joysticks[GLFW_JOYSTICK_LAST];
};

#endif