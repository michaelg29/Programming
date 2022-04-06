#version 330 core

#define M_PI 3.1415926535897932384626433832795

// take in points
layout (points) in;
// output triangles
// 16 edges for arm (32 vertices)
// 16+8 points for head (24 vertices)
layout (triangle_strip, max_vertices=56) out;

in VS_OUT {
	float mag;
	float radius;
	float head_radius;
	float head_height;
	vec3 color;

	mat4 model;
} gs_in[];

out vec4 fragColor;

uniform mat4 view;
uniform mat4 projection;

mat4 model;
int noEdges = 15;

void sendVertex(vec3 pos) {
	gl_Position = projection * view * model * vec4(pos, 1.0);
	EmitVertex();
}

void buildArm(float height, float radius) {
	float increment = 2 * M_PI / float(noEdges);

	for (float i = 0.0; i < float(noEdges) + 1.0; i += 1.0) {
		vec3 curXZ = vec3(radius * cos(i * increment), 0.0, radius * sin(i * increment));

		sendVertex(curXZ);
		sendVertex(vec3(curXZ.x, height, curXZ.z));
	}
	EndPrimitive();
}

void buildHead(float head_radius, float mag, float head_height) {
	float increment = 2 * M_PI / float(noEdges);

	bool sendHead = true;
	for (float i = 0.0; i < float(noEdges) + 1.0; i += 1.0) {
		vec3 curXZ = vec3(head_radius * cos(i * increment), mag - head_height, head_radius * sin(i * increment));

		if (sendHead) {
			sendVertex(vec3(0.0, mag, 0.0));
		}
		sendVertex(curXZ);
		sendHead = !sendHead;
	}
	EndPrimitive();
}

void main() {
	// extract data from vertex shader
	fragColor = vec4(gs_in[0].color, 1.0); // color for fragment shader
	model = gs_in[0].model; // transormation matrix

	// call build functions
	buildArm(gs_in[0].mag - gs_in[0].head_height, gs_in[0].radius);
	buildHead(gs_in[0].head_radius, gs_in[0].mag, gs_in[0].head_height);
}