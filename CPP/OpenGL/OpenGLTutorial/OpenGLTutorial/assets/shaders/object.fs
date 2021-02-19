struct Material {
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
uniform Material material;

uniform bool noTex;
uniform bool noNormal;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D normal0;

// UPDATE UNIFORM CALL
uniform sampler2D dirLightBuffer;
uniform samplerCube pointLightBuffers[MAX_POINT_LIGHTS];
uniform sampler2D spotLightBuffers[MAX_SPOT_LIGHTS];

out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
    vec2 TexCoord;

    TangentLights tanLights;
} fs_in;

// point light shadow offsets
#define NUM_SAMPLES 20
vec3 sampleOffsets[NUM_SAMPLES] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

// function signatures
vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcPointLight(int idx, vec3 norm, vec3 viewVec, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);

void main() {
	// =========================
	// SETUP PROPERTIES
	// =========================

	// normal texture
	vec3 norm = normalize(fs_in.tanLights.Normal);
	if (!noNormal) {
		// take normal vector from texture (in tangent space)
		norm = texture(normal0, fs_in.TexCoord).rgb;
		norm = normalize(norm * 2.0 - 1.0);
	}

	// diffuse and specular textures
	vec4 diffMap;
	vec4 specMap;
	if (noTex) {
		diffMap = material.diffuse;
		specMap = material.specular;
	} else {
		diffMap = texture(diffuse0, fs_in.TexCoord);
		specMap = texture(specular0, fs_in.TexCoord);
	}

	// calculate view vectors in tangent space
	vec3 viewVec = fs_in.tanLights.ViewPos - fs_in.tanLights.FragPos;
	vec3 viewDir = normalize(viewVec);
	// output placeholder
	vec4 result = vec4(0.0, 0.0, 0.0, 1.0);

	// =========================
	// CALCULATE LIGHTING
	// =========================

	// directional
	//result += calcDirLight(norm, viewDir, diffMap, specMap);

	// point lights
	//for (int i = 0; i < noPointLights; i++) {
		result += calcPointLight(0, norm, viewVec, viewDir, diffMap, specMap);
	//}

	// spot lights
	//for (int i = 0; i < noSpotLights; i++) {
		//result += calcSpotLight(i, norm, viewDir, diffMap, specMap);
	//}

	// =========================
	// APPLY ADJUSTMENTS
	// =========================

	// gamma correction
	float gamma = 2.2;
	result.rgb = pow(result.rgb, vec3(1.0 / gamma));

	// depth testing
	float near = 0.1;
	float far = 100.0;
	float z = gl_FragCoord.z * 2.0 - 1.0; // transform to NDC [0, 1] => [-1, 1]
	float linearDepth = (2.0 * near * far) / (z * (far - near) - (far + near)); // take inverse of the projection matrix (perspective)
	float factor = (near + linearDepth) / (near - far); // convert back to [0, 1]

	result.rgb *= 1 - factor;

	// =========================
	// RETURN
	// =========================

	FragColor = result;
}

float calcDirLightShadow(vec3 norm, vec3 lightDir) {
	// convert world coordinate to light space
	vec4 fragPosLightSpace = dirLight.lightSpaceMatrix * vec4(fs_in.FragPos, 1.0);

	// perspective divide (transform to NDC)
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // [near, far] => [-1, 1]

	// NDC to depth range
	projCoords = projCoords * 0.5 + 0.5; // [-1, 1] => [0, 1]

	if (projCoords.z > 1.0) {
		return 0.0;
	}

	// get depth of fragment
	float currentDepth = projCoords.z;

	// if greater than (closer), return 1, else return 0
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

	// blending
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(dirLightBuffer, 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcfDepth = texture(dirLightBuffer, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	// return average
	return shadow / 9.0;
}

vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
	// ambient
	vec4 ambient = dirLight.ambient * diffMap;

	// diffuse
	vec3 lightDir = normalize(-fs_in.tanLights.dirLightDirection);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = dirLight.diffuse * (diff * diffMap);

	// specular
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	if (diff > 0) {
		// if diff <= 0, object is "behind" light

		float dotProd = 0.0;
		// calculate using Blinn-Phong model
		vec3 halfwayDir = normalize(lightDir + viewDir);
		dotProd = dot(norm, halfwayDir);

		float spec = pow(max(dotProd, 0.0), material.shininess * 128);
		specular = dirLight.specular * (spec * specMap);
	}

	// calculate shadow in world space
	//float shadow = calcDirLightShadow(norm, lightDir);
	float shadow = 0.0;

	return vec4(ambient + (1.0 - shadow) * (diffuse + specular));
}

float calcPointLightShadow(int idx, vec3 norm, vec3 viewVec, vec3 lightDir) {
	// get vector from light to fragment
	vec3 lightToFrag = fs_in.FragPos - pointLights[idx].position;

	// get current depth
	float currentDepth = length(lightToFrag);

	// calculate bias
	float minBias = 0.005;
	float maxBias = 0.05;
	float bias = max(maxBias * (1.0 - dot(norm, lightDir)), minBias);

	// PCF WITH UNIFORM
	float shadow = 0.0;
	float viewDist = length(viewVec);
	float diskRadius = (1.0 + (viewDist / pointLights[idx].farPlane)) / 30.0;
	for (int i = 0; i < NUM_SAMPLES; i++) {
		float closestDepth = texture(pointLightBuffers[idx], lightToFrag + sampleOffsets[i] * diskRadius).r;
		closestDepth *= pointLights[idx].farPlane;
		if (currentDepth - bias > closestDepth) {
			shadow += 1.0;
		}
	}

	shadow /= 20.0;

	return shadow;
}

vec4 calcPointLight(int idx, vec3 norm, vec3 viewVec, vec3 viewDir, vec4 diffMap, vec4 specMap) {
	// ambient
	vec4 ambient = pointLights[idx].ambient * diffMap;

	// diffuse
	vec3 lightDir = normalize(fs_in.tanLights.pointLightPositions[idx] - fs_in.tanLights.FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec4 diffuse = pointLights[idx].diffuse * (diff * diffMap);

	// specular
	vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
	if (diff > 0) {
		// if diff <= 0, object is "behind" light

		float dotProd = 0.0;
		// calculate using Blinn-Phong model
		vec3 halfwayDir = normalize(lightDir + viewDir);
		dotProd = dot(norm, halfwayDir);

		float spec = pow(max(dotProd, 0.0), material.shininess * 128);
		specular = pointLights[idx].specular * (spec * specMap);
	}

	// calculate attenuation
	float dist = length(pointLights[idx].position - fs_in.FragPos);
	float attenuation = 1.0 / (pointLights[idx].k0 + pointLights[idx].k1 * dist + pointLights[idx].k2 * (dist * dist));

	// apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// calculate shadow in world space
	//float shadow = calcPointLightShadow(idx, norm, viewVec, lightDir);
	float shadow = 0.0;

	return vec4(ambient + (1.0 - shadow) * (diffuse + specular));
}

float calcSpotLightShadow(int idx, vec3 norm, vec3 lightDir) {
	vec4 fragPosLightSpace = spotLights[idx].lightSpaceMatrix * vec4(fs_in.FragPos, 1.0);

	// perspective divide (transform to NDC)
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; // [near, far] => [-1, 1]

	// NDC to depth range
	projCoords = projCoords * 0.5 + 0.5; // [-1, 1] => [0, 1]

	if (projCoords.z > 1.0) {
		return 0.0;
	}

	// get depth of fragment
	float currentDepth = projCoords.z;

	// if greater than (closer), return 1, else return 0
	float bias = max(0.05 * (1.0 - dot(norm, lightDir)), 0.005);

	// blending
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(spotLightBuffers[idx], 0);
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			float pcfDepth = texture(spotLightBuffers[idx], projCoords.xy + vec2(x, y) * texelSize).r;
			pcfDepth *= spotLights[idx].farPlane;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}

	return shadow / 9.0;
}

vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap) {
	vec3 lightDir = normalize(fs_in.tanLights.spotLightPositions[idx] - fs_in.tanLights.FragPos);
	float theta = dot(lightDir, normalize(-fs_in.tanLights.spotLightDirections[idx]));

	// ambient
	vec4 ambient = spotLights[idx].ambient * diffMap;

	if (theta > spotLights[idx].outerCutOff) {
		// if in cutoff, render light
		// > because using cosines and not degrees

		// diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec4 diffuse = spotLights[idx].diffuse * (diff * diffMap);

		// specular
		vec4 specular = vec4(0.0, 0.0, 0.0, 1.0);
		if (diff > 0) {
			// if diff <= 0, object is "behind" light

			float dotProd = 0.0;
			// calculate using Blinn-Phong model
			vec3 halfwayDir = normalize(lightDir + viewDir);
			dotProd = dot(norm, halfwayDir);

			float spec = pow(max(dotProd, 0.0), material.shininess * 128);
			specular = spotLights[idx].specular * (spec * specMap);
		}

		// calculate Intensity
		float intensity = clamp((theta - spotLights[idx].outerCutOff) / (spotLights[idx].cutOff - spotLights[idx].outerCutOff), 0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;

		// calculate attenuation
		float dist = length(spotLights[idx].position - fs_in.FragPos);
		float attenuation = 1.0 / (spotLights[idx].k0 + spotLights[idx].k1 * dist + spotLights[idx].k2 * (dist * dist));

		// apply attenuation
		ambient *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;

		//float shadow = calcSpotLightShadow(idx, norm, lightDir);
		float shadow = 0.0;

		return vec4(ambient + (1.0 - shadow) * (diffuse + specular));
	}
	else {
		// render just ambient
		return ambient;
	}
}