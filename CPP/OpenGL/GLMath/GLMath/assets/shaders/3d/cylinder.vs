#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 offset;
layout (location = 4) in vec3 size;
layout (location = 5) in vec3 diff;
layout (location = 6) in vec4 spec;

out vec3 fragPos;
out vec3 normal;

out vec4 fragColor;
out vec4 diffuse;
out vec4 specular;
out float shininess;

uniform mat4 projView;

void main() {
	fragPos = pos * size + offset;
	gl_Position = projView * vec4(fragPos, 1.0);
	normal = norm;

	fragColor = vec4(diff, 1.0);
	diffuse = vec4(diff, 1.0);
	specular = vec4(spec.rgb, 1.0);
	shininess = spec.a;
}