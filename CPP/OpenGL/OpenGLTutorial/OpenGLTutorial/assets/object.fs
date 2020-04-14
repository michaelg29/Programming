#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

#define MAX_POINT_LIGHTS 20
struct PointLight {    
    vec3 position;
    
    float k0;
    float k1;
    float k2;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int noPointLights;

#define MAX_SPOT_LIGHTS 5
struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float k0;
    float k1;
    float k2; 
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int noSpotLights;

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform Material material;

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 tex);
vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 tex);
vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 tex);

void main() {
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// texture
	vec3 tex = vec3(texture(material.diffuse, TexCoord));

	vec3 result;

	// directional
	result = calcDirLight(dirLight, norm, viewDir, tex);

	// point lights
	for (int i = 0; i < noPointLights; i++) {
		result += calcPointLight(pointLights[i], norm, viewDir, tex);
	}

	// spot lights
	for (int i = 0; i < noSpotLights; i++) {
		result += calcSpotLight(spotLights[i], norm, viewDir, tex);
	}

	FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 tex) {
	// ambient
	vec3 ambient = light.ambient * tex;

	// diffuse lighting
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	return vec3(ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 viewDir, vec3 tex) {
	// ambient
	vec3 ambient = light.ambient * tex;

	// diffuse lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoord)));

	float dist = length(light.position - FragPos);
	float attenuation = 1.0 / (light.k0 + light.k1 * dist + light.k2 * (dist * dist));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec3(ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 viewDir, vec3 tex) {
	vec3 lightDir = normalize(light.position - FragPos);

	// check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction));

	// ambient
	vec3 ambient = light.ambient * tex;

	if (theta > light.outerCutOff) { // > because dot returns cosines, which is inversely related to angle
		// diffuse lighting
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoord)));

		// specular
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

		return vec3(ambient + diffuse + specular);
	} else {
		// else, use ambient light so scene isn't completely dark outside the spotlight.
        return ambient;
	}
}