#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform vec3 lightColor;
uniform vec3 lightPos;

uniform float mixVal;

void main() {
	// ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	//FragColor = vec4(ambient, 1.0) * mix(texture(texture0, TexCoord), texture(texture1, TexCoord), mixVal);

	// diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	FragColor = vec4(ambient + diffuse, 1.0) * mix(texture(texture0, TexCoord), texture(texture1, TexCoord), mixVal);
}