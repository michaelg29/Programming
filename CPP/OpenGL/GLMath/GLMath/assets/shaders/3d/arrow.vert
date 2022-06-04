#version 330 core

layout (location = 0) in vec4 dimensions;
layout (location = 1) in vec4 color;
layout (location = 2) in mat4 model;
layout (location = 6) in mat3 normModel;
layout (location = 9) in vec3 diffuse;
layout (location = 10) in vec3 specular;

out VS_OUT {
	float mag; // length of arrow
	float radius; // radius of arm
	float head_radius; // radius of head
	float head_height; // height of head
	mat4 model; // transformation matrix
	mat3 normalModel;

	vec3 color;
	vec3 diffuse;
	vec3 specular;
	float shininess;
} vs_out;

void main() {
	// pass data to geometry shader
	vs_out.mag = dimensions.x;
	vs_out.radius = dimensions.y;
	vs_out.head_radius = dimensions.z;
	vs_out.head_height = dimensions.w;
	vs_out.model = model;
	vs_out.normalModel = normModel;

	vs_out.color = color.xyz;
	vs_out.diffuse = diffuse;
	vs_out.specular = specular;
	vs_out.shininess = color.w;
}