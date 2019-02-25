#include "libs.h"

int main() {
	glfwInit();

	glm::vec3(0.f);

	// GLEW
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwTerminate();
		throw "Error in GLEW Init";
	}

	system("pause");
	return 0;
}