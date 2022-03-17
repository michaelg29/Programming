#version 330 core

// take in points
layout (points) in;
// output line
layout (line_strip, max_vertices=2) out;

in VS_OUT {
	vec2 pt;
	vec2 dir;
	vec4 fragColor;
} gs_in[];

out vec4 fragColor;

void main() {
	fragColor = gs_in[0].fragColor;

	gl_Position = vec4(gs_in[0].pt, 0.0, 1.0);
	EmitVertex();
	gl_Position += vec4(gs_in[0].dir, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
}