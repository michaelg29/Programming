#ifndef GLMEMORY_H
#define GLMEMORY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <map>

/*
	Structure for buffer objects
	- VBO, EBO
*/
class BufferObject {
public:
	// value/location of object
	GLuint val;
	// type of buffer (GL_ARRAY_BUFFER || GL_ELEMENT_ARRAY_BUFFER)
	GLenum type;

	BufferObject() {}

	BufferObject(GLenum type)
		: type(type) {}

	// generate object
	void generate() {
		glGenBuffers(1, &val);
	}

	// bind object
	void bind() {
		glBindBuffer(type, val);
	}

	// set data (glBufferData)
	template<typename T>
	void setData(GLuint noElements, T* data, GLenum usage) {
		glBufferData(type, noElements * sizeof(T), (void*)data, usage);
	}

	// update data (glBufferSubData)
	template<typename T>
	void updateData(GLintptr offset, GLuint noElements, T* data) {
		glBufferSubData(type, offset, noElements * sizeof(T), (void*)data);
	}

	// set vertex attribute pointer
	template<typename T>
	void setAttPointer(GLuint idx, GLint size, GLenum type, unsigned int stride, unsigned int offset, GLuint divisor = 0) {
		glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
		glEnableVertexAttribArray(idx);
		if (divisor > 0) {
			// reset _idx_ attribute every _divisor_ instances
			glVertexAttribDivisor(idx, divisor);
		}
	}

	// clear buffer objects (ie bind 0)
	void clear() {
		glBindBuffer(type, 0);
	}

	// cleanup
	void cleanup() {
		glDeleteBuffers(1, &val);
	}
};

/*
	Structure for array objects
	- VAO
*/
class ArrayObject {
public:
	// value/location of object
	GLuint val;

	std::map<const char*, BufferObject> buffers;

	// get buffer
	BufferObject& operator[](const char* key) {
		return buffers[key];
	}

	// generate object
	void generate() {
		glGenVertexArrays(1, &val);
	}

	// bind object
	void bind() {
		glBindVertexArray(val);
	}

	// draw
	void draw(GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instancecount = 1) {
		glDrawElementsInstanced(mode, count, type, (void*)indices, instancecount);
	}

	// cleanup
	void cleanup() {
		glDeleteVertexArrays(1, &val);
		for (auto& pair : buffers) {
			pair.second.cleanup();
		}
	}

	// clear array objects (ie bind 0)
	static void clear() {
		glBindVertexArray(0);
	}
};

#endif