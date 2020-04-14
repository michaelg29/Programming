#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
	vec3 position; // point light/spotlight
	vec3 direction; // directional light/spotlight
	float cutOff; // spotlight
	float outerCutOff; // spotlight outer bound
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	// attenuation constants
	float k0;
    float k1;
    float k2;
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
	vec3 lightDir = normalize(light.position - FragPos); // point light
	//vec3 lightDir = normalize(-light.direction); // directional light

	// check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));

	if (theta > light.outerCutOff) { // > because dot returns cosines, which is inversely related to angle
		// ambient lighting
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));

		// diffuse lighting
		vec3 norm = normalize(Normal);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

		// specular
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
		vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

		// calculate intensity
		float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);
		diffuse  *= intensity;
		specular *= intensity;

		// calculate attenuation
		float dist = length(light.position - FragPos);
		float attenuation = 1.0 / (light.k0 + light.k1 * dist + light.k2 * (dist * dist));

		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
	} else {
		// else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoord), 1.0);
	}
}