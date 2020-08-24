#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform float time;

void main() {
	//FragColor = texture(skybox, TexCoords);
	vec3 color = vec3(-0.5 * cos(time) + 0.5);
	FragColor = vec4(color, 1.0);
}