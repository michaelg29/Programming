#version 330 core

// take in points
layout (points) in;
// output parallelogram
layout (triangle_strip, max_vertices=4) out;

out vec4 fragColor;
out vec4 diffuse;
out vec4 specular;
out float shininess;

out vec3 fragPos;
out vec3 normal;

layout (std140) uniform SurfaceUniform {
	vec4 bounds;
	vec4 dim;
	vec3 s_ambient;
	vec3 s_diffuse;
	vec3 s_specular;
	float s_shininess;
};

in VS_OUT {
	int idx;
} gs_in[];

uniform mat4 view;
uniform mat4 projection;

float y_min;
float y_max;

void sendVertex(vec3 pos, vec3 norm) {
	fragPos = pos;
	normal = norm;
	gl_Position = projection * view * vec4(pos, 1.0);
	EmitVertex();
}

float func(float x, float z) {
	return 1 / (x*x + z*z);
}

vec3 norm(float x, float z) {
	float denom = func(x, z);
	denom *= denom;

	return vec3(2 * x * denom, 2 * z * denom, 1);
}

void calcAndSendPoint(float x, float z) {
	float y = func(x, z);
	vec3 norm = vec3(2 * x * y * y, 1, 2 * z * y * y);

	if (y < y_min) {
		y = y_min;
		norm = vec3(0.0, -1.0, 0.0);
	}
	else if (y > y_max) {
		y = y_max;
		norm = vec3(0.0, 1.0, 0.0);
	}

	sendVertex(vec3(x, y, z), norm);
}

uniform int x_len;
uniform int y_len;

void main() {
	fragColor = vec4(s_ambient, 1.0);
	diffuse = vec4(s_diffuse, 1.0);
	specular = vec4(s_specular, 1.0);
	shininess = s_shininess;

	// x_inc, y_inc, z_min, z_max
	float x_inc = dim.x;
	float z_inc = dim.y;
	y_min = dim.z;
	y_max = dim.w;

	// convert index into cell
	float x = (float(gs_in[0].idx % x_len) * x_inc) + bounds.x;
	float z = (float(gs_in[0].idx / y_len) * z_inc) + bounds.y;

	calcAndSendPoint(x, z);
	calcAndSendPoint(x + x_inc, z);
	calcAndSendPoint(x, z + z_inc);
	calcAndSendPoint(x + x_inc, z + z_inc);
	EndPrimitive();
}