#version 330 core

in vec4 fragColor;
in vec4 diffuse;
in vec4 specular;
in float shininess;

in vec3 fragPos;
in vec3 normal;
out vec4 color;

uniform vec3 viewPos;

struct DirLight {
    vec3 direction;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

layout (std140) uniform DirLightUniform {
    DirLight dirLight;
};

vec4 calcDirLight(DirLight dirLight, vec3 norm, vec3 viewVec, vec4 diffMap, vec4 specMap);

void main() {
    color = vec4(0.0, 0.0, 0.0, 1.0);

    color += calcDirLight(dirLight, normal, viewPos - fragPos, diffuse, specular);
    //color = fragColor;
}

vec4 calcDirLight(DirLight dirLight, vec3 norm, vec3 viewVec, vec4 diffMap, vec4 specMap) {
    vec3 viewDir = normalize(viewVec);
    // ambient
    vec4 ambient = dirLight.ambient * diffMap;

    // diffuse
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = dirLight.diffuse * (diff * diffMap);

    // specular
    vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
    if (diff > 0) {
        // if diff <= 0, object "behind light"
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float dotProd = dot(norm, halfwayDir);

        float spec = pow(max(dotProd, 0.0), shininess * 128);
        specular = dirLight.specular * (spec * specMap);
    }

    return vec4(ambient + diffuse + specular);
}