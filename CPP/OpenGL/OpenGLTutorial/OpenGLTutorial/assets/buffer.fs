#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D bufferTex;

float near = 0.1;
float far = 100.0;

// required when using a perspective projection matrix
float linearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0;
	float linearDepth = (2.0 * near * far) / (depth * (far - near) - (far + near));	

    return linearDepth;
}

void main()
{        
    // DEPTH MAP
    float depthValue = texture(bufferTex, TexCoords).r;
    float linearDepth = linearizeDepth(depthValue);
    FragColor = vec4(vec3((1 - (near + linearDepth) / (near - far))), 1.0); // perspective
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic

    // COLOR MAP
    //FragColor = vec4(texture(bufferTex, TexCoords).rgb, 1.0);
}
