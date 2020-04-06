#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float mixVal;

void main() {
	FragColor = texture(texture0, TexCoord);
}