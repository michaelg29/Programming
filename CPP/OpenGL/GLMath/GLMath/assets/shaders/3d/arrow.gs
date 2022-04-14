#version 330 core

#define M_PI 3.1415926535897932384626433832795

// take in points
layout (points) in;
// output triangles
// 16 edges for arm (32 vertices)
// 16+8 points for head (24 vertices)
layout (triangle_strip, max_vertices=56) out;

in VS_OUT {
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
} gs_in[];

out vec3 fragPos;
out vec3 normal;
out vec4 fragColor;
out vec4 diffuse;
out vec4 specular;
out float shininess;

uniform mat4 view;
uniform mat4 projection;

mat4 model;
mat3 normalModel;

int noEdges = 15;

void sendVertex(vec3 pos, vec3 norm) {
	vec4 transformedPos = model * vec4(pos, 1.0);
	fragPos = transformedPos.xyz;
	normal = normalModel * norm;
	gl_Position = projection * view * transformedPos;
	EmitVertex();
}

void buildArm(float height, float radius) {
	float increment = 2 * M_PI / float(noEdges);

	for (float i = 0.0; i < float(noEdges) + 1.0; i += 1.0) {
		vec3 curXZ = vec3(radius * cos(i * increment), 0.0, radius * sin(i * increment));
		vec3 norm = curXZ / radius;

		sendVertex(curXZ, norm);
		sendVertex(vec3(curXZ.x, height, curXZ.z), norm);
	}
	EndPrimitive();
}

void buildHead(float head_radius, float mag, float head_height) {
	float increment = 2 * M_PI / float(noEdges);

	bool sendHead = true;
	for (float i = 0.0; i < float(noEdges) + 1.0; i += 1.0) {
		vec3 curXZ = vec3(head_radius * cos(i * increment), mag - head_height, head_radius * sin(i * increment));
		vec3 norm = vec3(cos(i * increment), 1.0 / (head_height / head_radius), sin(i * increment));

		if (sendHead) {
			sendVertex(vec3(0.0, mag, 0.0), norm);
		}
		sendVertex(curXZ, norm);
		sendHead = !sendHead;
	}
	EndPrimitive();
}

void main() {
	// extract data from vertex shader
	fragColor = vec4(gs_in[0].color, 1.0); // color for fragment shader
	diffuse = vec4(gs_in[0].diffuse, 1.0);
	specular = vec4(gs_in[0].specular, 1.0);
	shininess = gs_in[0].shininess;
	
	model = gs_in[0].model; // transormation matrix
	normalModel = gs_in[0].normalModel;

	// call build functions
	buildArm(gs_in[0].mag - gs_in[0].head_height, gs_in[0].radius);
	buildHead(gs_in[0].head_radius, gs_in[0].mag, gs_in[0].head_height);
}