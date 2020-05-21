#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aOffset;
layout (location = 4) in vec3 aSize;

// instanced replaces this
//uniform vec3 offsets[100];

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	//vec3 pos = vec3(aPos.x * aSize.x, aPos.y * aSize.y, aPos.z * aSize.z);
	//gl_Position = projection * view * model * vec4(pos + aOffset, 1.0);
	//fColor = aColor;


	vec3 pos;
	if (aSize.x == 0 || aSize.y == 0 || aSize.z == 0) {
		pos = aPos;
	}
	else {
		pos = vec3(aPos.x * aSize.x, aPos.y * aSize.y, aPos.z * aSize.z);
	}

	FragPos = vec3(model * vec4(pos + aOffset, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;

	gl_Position = projection * view * vec4(FragPos, 1.0);
	TexCoord = aTexCoord;
}