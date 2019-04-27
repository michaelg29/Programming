#ifndef JOYSTICK
#define JOYSTICK

#define GLFW_JOYSTICK_LEFT_STICK_X 0
#define GLFW_JOYSTICK_LEFT_STICK_Y 1
#define GLFW_JOYSTICK_RIGHT_STICK_X 2
#define GLFW_JOYSTICK_RIGHT_STICK_Y 3
#define GLFW_JOYSTICK_LEFT_TRIGGER 4
#define GLFW_JOYSTICK_RIGHT_TRIGGER 5

#define GLFW_JOYSTICK_PS_SQUARE 0
#define GLFW_JOYSTICK_PS_X 1
#define GLFW_JOYSTICK_PS_CIRCLE 2
#define GLFW_JOYSTICK_PS_TRIANGLE 3
#define GLFW_JOYSTICK_PS_L1 4
#define GLFW_JOYSTICK_PS_R1 5
#define GLFW_JOYSTICK_PS_L2 6
#define GLFW_JOYSTICK_PS_R2 7
#define GLFW_JOYSTICK_PS_SHARE 8
#define GLFW_JOYSTICK_PS_OPTIONS 9
#define GLFW_JOYSTICK_PS_L3 10
#define GLFW_JOYSTICK_PS_R3 11
#define GLFW_JOYSTICK_PS_HOME 12
#define GLFW_JOYSTICK_PS_CLICK 13
#define GLFW_JOYSTICK_PS_UP 14
#define GLFW_JOYSTICK_PS_RIGHT 15
#define GLFW_JOYSTICK_PS_DOWN 16
#define GLFW_JOYSTICK_PS_LEFT 17

#define GLFW_JOYSTICK_XBOX_X 0
#define GLFW_JOYSTICK_XBOX_A 1
#define GLFW_JOYSTICK_XBOX_B 2
#define GLFW_JOYSTICK_XBOX_Y 3
#define GLFW_JOYSTICK_XBOX_LB 4
#define GLFW_JOYSTICK_XBOX_RB 5
#define GLFW_JOYSTICK_XBOX_LT 6
#define GLFW_JOYSTICK_XBOX_RT 7
#define GLFW_JOYSTICK_XBOX_ADDRESS 8
#define GLFW_JOYSTICK_XBOX_MENU 9
#define GLFW_JOYSTICK_XBOX_LS 10
#define GLFW_JOYSTICK_XBOX_RS 11
#define GLFW_JOYSTICK_XBOX_HOME 12
//#define GLFW_JOYSTICK_XBOX_CLICK 13
#define GLFW_JOYSTICK_XBOX_UP 14
#define GLFW_JOYSTICK_XBOX_RIGHT 15
#define GLFW_JOYSTICK_XBOX_DOWN 16
#define GLFW_JOYSTICK_XBOX_LEFT 17

class Joystick {
public:
	Joystick();
	Joystick(int i);
	void Update();

	float TriggerState(int trigger);
	unsigned char ButtonState(int button);

	static int GetId(int i);

private:
	int present;

	int id;
	const char* name;
	int axesCount;
	const float* axes;
	int buttonCount;
	const unsigned char* buttons;
};

#endif