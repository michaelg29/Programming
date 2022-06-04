#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec3 size;
layout (location = 3) in vec3 diff;
layout (location = 4) in vec4 spec;

out vec2 tex;
out vec3 fragPos;
out vec3 normal;
out vec3 diffMap;
out vec3 specMap;
out float shininess;

uniform mat4 projView;

void main() {
	fragPos = pos * size + offset;
	gl_Position = projView * vec4(fragPos, 1.0);
	normal = pos;

	diffMap = diff;
	specMap = spec.rgb;
	shininess = spec.a;
}