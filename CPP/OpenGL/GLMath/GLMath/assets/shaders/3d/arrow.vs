#version 330 core

layout (location = 0) in vec3 start;
layout (location = 1) in vec3 end;
layout (location = 2) in vec3 dimensions;
layout (location = 3) in vec3 color;
layout (location = 4) in mat4 model;

out VS_OUT {
	vec3 start;
	vec3 end;
	float radius;
	float head_radius;
	float head_height;
	vec3 color;
	mat4 model;
} vs_out;

void main() {
	vs_out.start = start;
	vs_out.end = end;
	vs_out.radius = dimensions.x;
	vs_out.head_radius = dimensions.y;
	vs_out.head_height = dimensions.z;
	vs_out.color = color;
	vs_out.model = model;
}