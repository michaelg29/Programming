#ifndef ENGINE
#define ENGINE

#include "IO/Joystick.h"

#include "GLFW/glfw3.h"
#pragma comment(lib, "opengl32.lib")

class Engine {
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static double GetDT();

	Engine();
	~Engine();

	bool Initialize(const char* windowTitle);

	void Update();
	void BeginRender();
	void EndRender();

	Joystick GetJoystick(int id);

private:
	static GLFWwindow* window;

	static double dt;
	double lastTime;

	Joystick joysticks[GLFW_JOYSTICK_LAST];
};

#endif