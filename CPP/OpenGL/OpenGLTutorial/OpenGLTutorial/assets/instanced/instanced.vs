#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec3 aSize;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
	FragPos = aPos * aSize + aOffset;

	Normal = mat3(transpose(inverse(mat4(1.0)))) * aNormal;

	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
}