#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
	//vec3 position;
	vec3 direction; // directional light
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform float mixVal;
uniform Material material;
uniform Light light;

void main() {
	// ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

	// diffuse lighting
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
}