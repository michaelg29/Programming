#version 330 core

// take in points
layout (points) in;
// output parallelogram
layout (triangle_strip, max_vertices=4) out;

in VS_OUT {
	int idx;

	vec2 minBound;
	vec2 maxBound;

	float x_inc;
	float z_inc;

	vec3 diffuse;
	vec3 specular;
	float shininess;
} gs_in[];

out vec2 tex;
out vec3 fragPos;
out vec3 normal;
out vec3 diffMap;
out vec3 specMap;
out float shininess;

uniform mat4 projView;

void sendVertex(vec3 pos, vec3 norm) {
	fragPos = pos;
	normal = norm;
	gl_Position = projView * vec4(pos, 1.0);
	EmitVertex();
}

float func(float x, float z) {
	float denom = x * x + z * z;
	if (denom == 0.0) {
		return 300.0;
	}

	return 1 / denom;
}

vec3 norm(float x, float z) {
	float denom = func(x, z);
	denom *= denom;

	return vec3(2 * x * denom, 2 * z * denom, 1);
}

void calcAndSendPoint(float x, float z) {
	float y = func(x, z);
	vec3 norm = vec3(2 * x * y * y, 1, 2 * z * y * y);

	sendVertex(vec3(x, y, z), norm);
}

uniform int x_cells;
uniform int z_cells;

void main() {
	diffMap = gs_in[0].diffuse;
	specMap = gs_in[0].specular;
	shininess = gs_in[0].shininess;

	// convert index into cell
	float x = (float(gs_in[0].idx % x_cells) * gs_in[0].x_inc) + gs_in[0].minBound.x;
	float z = (float(gs_in[0].idx / z_cells) * gs_in[0].z_inc) + gs_in[0].minBound.y;

	calcAndSendPoint(x, z);
	calcAndSendPoint(x + gs_in[0].x_inc, z);
	calcAndSendPoint(x, z + gs_in[0].z_inc);
	calcAndSendPoint(x + gs_in[0].x_inc, z + gs_in[0].z_inc);
	EndPrimitive();
}