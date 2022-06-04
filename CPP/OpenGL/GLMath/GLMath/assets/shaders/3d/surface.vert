#version 330 core

layout (location = 0) in vec4 bounds;
layout (location = 1) in vec2 inc;
layout (location = 2) in vec3 diffuse;
layout (location = 3) in vec4 specular;

out VS_OUT {
	int idx;

	vec2 minBound;
	vec2 maxBound;

	float x_inc;
	float z_inc;

	vec3 diffuse;
	vec3 specular;
	float shininess;
} vs_out;

void main() {
	vs_out.idx = gl_VertexID;

	vs_out.minBound = bounds.xy;
	vs_out.maxBound = bounds.zw;

	vs_out.x_inc = inc.x;
	vs_out.z_inc = inc.y;

	vs_out.diffuse = diffuse;
	vs_out.specular = specular.rgb;
	vs_out.shininess = specular.a;
}