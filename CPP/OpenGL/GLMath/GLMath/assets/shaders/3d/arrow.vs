#version 330 core

layout (location = 0) in vec4 dimensions;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 model;

out VS_OUT {
	float mag; // length of arrow
	float radius; // radius of arm
	float head_radius; // radius of head
	float head_height; // height of head
	vec3 color; // color
	mat4 model; // transformation matrix
} vs_out;

void main() {
	// pass data to geometry shader
	vs_out.mag = dimensions.x;
	vs_out.radius = dimensions.y;
	vs_out.head_radius = dimensions.z;
	vs_out.head_height = dimensions.w;
	vs_out.color = color;
	vs_out.model = model;
}