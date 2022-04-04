#version 330 core

struct Axis {
	float lineIncrement;
	vec3 color;
};

layout (std140) uniform Axes {
	Axis axes[2];
};

out VS_OUT {
	vec2 pt;
	vec2 dir;
	vec4 fragColor;
} vs_out;

void main() {
	// draw main axes
	vs_out.fragColor = vec4(axes[gl_VertexID].color, 1.0);
	if (gl_VertexID == 0) {
		vs_out.pt = vec2(0.0, -1.0);
		vs_out.dir = vec2(0.0, 2.0);
	}
	else {
		vs_out.pt = vec2(-1.0, 0.0);
		vs_out.dir = vec2(2.0, 0.0);
	}
}