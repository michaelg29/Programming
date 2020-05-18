#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>

class Shader {
public:
	// program ID
	unsigned int id;

	// contructor
	Shader();
	Shader(const char* vertexShaderPath, const char* fragShaderPath, const char* geoShaderPath = nullptr);

	bool generateProgram(const char* vertexShaderPath, const char* fragShaderPath, const char* geoShaderPath = "");

	// activate shader
	void activate();

	// utility functions
	std::string loadShaderSrc(const char* filePath);
	GLuint compileShader(const char* filePath, GLuint type);

	// uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void set3Float(const std::string& name, glm::vec3 vec3);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void set4Float(const std::string& name, glm::vec4 vec4);
	void set4Float(const std::string& name, aiColor4D color);
	void setMat4(const std::string& name, glm::mat4 val);
};

#endif