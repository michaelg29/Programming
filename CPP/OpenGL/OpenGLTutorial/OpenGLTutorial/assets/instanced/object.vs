#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aOffset;
layout (location = 2) in vec3 aSize;
//out vec3 fColor;

// instanced replaces this
//uniform vec3 offsets[100];

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec3 pos = vec3(aPos.x * aSize.x, aPos.y * aSize.y, aPos.z * aSize.z);
	gl_Position = projection * view * model * vec4(pos + aOffset, 1.0);
	//fColor = aColor;
}