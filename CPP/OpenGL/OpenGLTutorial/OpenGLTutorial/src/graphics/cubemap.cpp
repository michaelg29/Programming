#include "cubemap.h"

Cubemap::Cubemap() 
	: hasTextures(false) {}

void Cubemap::loadTextures(std::string _dir,
	std::string right,
	std::string left,
	std::string top,
	std::string bottom,
	std::string front,
	std::string back) {
	dir = _dir;
	hasTextures = true;
	faces = { right, left, top, bottom, front, back };

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	int width, height, nChannels;

	for (unsigned int i = 0, len = faces.size(); i < len; i++) {
		unsigned char* data = stbi_load((dir + "/" + faces[i]).c_str(), &width, &height, &nChannels, 0);

		GLenum colorMode = GL_RED;
		switch (nChannels) {
		case 3:
			colorMode = GL_RGB;
			break;
		case 4:
			colorMode = GL_RGBA;
			break;
		}

		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Failed to load texture at " << faces[i] << std::endl;
		}

		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::init() {
	/*
		set up vertices
	*/

	unsigned int noVertices = 36;

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// bind VAO
	VAO.generate();
	VAO.bind();

	// load data into vertex buffers
	VAO["VBO"] = BufferObject(GL_ARRAY_BUFFER);
	VAO["VBO"].generate();
	VAO["VBO"].bind();
	VAO["VBO"].setData<float>(noVertices * 3, skyboxVertices, GL_STATIC_DRAW);

	// set the vertex attribute pointers
	VAO["VBO"].bind();
	// vertex Positions
	VAO["VBO"].setAttPointer<GLfloat>(0, 3, GL_FLOAT, 3, 0);

	VAO["VBO"].clear();

	ArrayObject::clear();
}

void Cubemap::render(Shader shader, Scene* scene) {
	glDepthMask(GL_FALSE);
	shader.activate();

	// remove translation from view matrix
	glm::mat4 view = glm::mat4(glm::mat3(scene->getActiveCamera()->getViewMatrix()));
	shader.setMat4("view", view);
	shader.setMat4("projection", scene->projection);

	if (hasTextures) {
		glBindTexture(GL_TEXTURE_CUBE_MAP, id);
	}

	VAO.bind();
	VAO.draw(GL_TRIANGLES, 0, 36);
	ArrayObject::clear();

	glDepthMask(GL_TRUE);
}

void Cubemap::cleanup() {
	VAO.cleanup();
}