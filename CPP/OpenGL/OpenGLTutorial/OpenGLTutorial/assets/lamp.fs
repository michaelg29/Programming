#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

uniform int bullet;

void main() {
	if (bullet == 1) {
		FragColor = vec4(0.05, 0.05, 0.05, 1.0);
	} else {
		FragColor = vec4(lightColor, 1.0); // set to white
	}
}