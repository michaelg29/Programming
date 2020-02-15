#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // set in code
//in vec3 ourColor;
in vec2 TexCoord;

uniform float mixValue; // set in code

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = ourColor;
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue); // 20% of first, 80% of second
}