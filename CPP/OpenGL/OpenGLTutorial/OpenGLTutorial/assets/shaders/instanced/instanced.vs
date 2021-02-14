layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec3 aOffset;
layout (location = 6) in vec3 aSize;

#define MAX_POINT_LIGHTS 20
#define MAX_SPOT_LIGHTS 5

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;

    vec3 ViewPos;

    TangentLightData lightData;
} vs_out;

layout (std140) uniform Matrices {
    mat4 model2[3];
};

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

void main() {
    vec3 pos = aPos * aSize + aOffset;

    vs_out.FragPos = vec3(model * model2[2] * vec4(pos, 1.0));
    vs_out.Normal = normalModel * aNormal;

    vec3 T = normalize(normalModel * aTangent);
    vec3 B = normalize(normalModel * aBitangent);
    vec3 N = normalize(normalModel * aNormal);
    mat3 TBNinv = transpose(mat3(T, B, N)); // orthogonal matrix => transpose = inverse

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
    vs_out.TexCoord = aTexCoord;

    // transform lights to tangent space
    vs_out.FragPos = TBNinv * vs_out.FragPos;
    vs_out.ViewPos = TBNinv * viewPos;
}