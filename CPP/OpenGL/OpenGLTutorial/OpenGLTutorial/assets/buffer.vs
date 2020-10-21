#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 4) in vec3 aSize;

out vec2 TexCoords;

void main() {
	TexCoords = aTexCoords;
	gl_Position = vec4(aSize * aPos, 1.0);
}