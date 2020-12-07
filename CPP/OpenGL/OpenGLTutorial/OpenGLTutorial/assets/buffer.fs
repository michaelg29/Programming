#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D bufferTex;

float near = 0.1;
float far = 100.0;

// required when using a perspective projection matrix
float linearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // transform to NDC [0, 1] => [-1, 1]
	float linearDepth = (2.0 * near * far) / (depth * (far - near) - (far + near));	

    return linearDepth;
}

void main()
{        
    // DEPTH MAP
    float depthValue = texture(bufferTex, TexCoords).r; // take value from depth buffer
    float linearDepth = linearizeDepth(depthValue); // take inverse of the projection matrix (perspective)
    float factor = (near + linearDepth) / (near - far); // convert back to [0, 1]
    FragColor = vec4(vec3(1 - factor), 1.0); // set color
    //FragColor = vec4(vec3(depthValue), 1.0); // orthographic

    // COLOR MAP
    //FragColor = vec4(texture(bufferTex, TexCoords).rgb, 1.0); // take value from color buffer
}
