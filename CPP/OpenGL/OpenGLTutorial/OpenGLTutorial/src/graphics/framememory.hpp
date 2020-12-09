#ifndef FRAMEMEMORY_HPP
#define FRAMEMEMORY_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>

#include "texture.h"

class RenderbufferObject {
public:
	GLuint val;

	void generate() {
		glGenRenderbuffers(1, &val);
	}

	void bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, val);
	}

	void allocate(GLenum format, GLuint width, GLuint height) {
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}
};

class FramebufferObject {
public:
	GLuint fbo;
	GLuint width;
	GLuint height;
	GLbitfield bitCombo;

	FramebufferObject()
		: fbo(0), width(0), height(0), bitCombo(0) {}

	FramebufferObject(GLuint width, GLuint height, GLbitfield bits)
		: fbo(0), width(width), height(height), bitCombo(bits) {}

	void generate() {
		glGenFramebuffers(1, &fbo);
	}

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	void setViewport() {
		glViewport(0, 0, width, height);
	}

	void clear() {
		glClear(bitCombo);
	}

	void activate() {
		bind();
		setViewport();
		clear();
	}

	void attachRBO(GLenum attachType, RenderbufferObject rbo) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachType, GL_RENDERBUFFER, rbo.val);
	}

	void attachTexture(GLenum attachType, Texture tex) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, tex.id, 0);
	}
};

#endif