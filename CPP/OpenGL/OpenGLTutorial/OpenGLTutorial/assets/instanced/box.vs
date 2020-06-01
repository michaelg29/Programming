#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aOffset;
layout (location = 2) in vec3 aSize;

uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * vec4(aPos * aSize + aOffset, 1.0);
}