#version 330 core

out vec4 fragColour;

in vec3 vertexPos;
in vec3 normal;
in vec4 camPos;
in vec2 texCoord;

uniform sampler2D texSampler;

struct Light {
	vec4 direction;
	vec4 intensities;
	float ambient;
};

#define MAX_LIGHTS 20
layout (std140) uniform Lights 
{

	Light lights[MAX_LIGHTS];
	int numLights;
};


vec3 applyLight(Light currLight, vec3 vertexPos, vec3 vertexColour, vec3 surfaceToCamera)
{
	//This is for directional lights only right now
	float attenuation = 1.0f;

	//Calculate ambient lighting from the current vtx colour, intensity of light, and ambience
	vec3 ambient = vertexColour.rgb *currLight.ambient * currLight.intensities.xyz;

	//Diffuse
	float diffuseCoefficient = max(0.0f, dot(normal.xyz, currLight.direction.xyz));
	vec3 diffuse = vertexColour.rgb * diffuseCoefficient * currLight.intensities.xyz;

	float specularCoefficient = 0.0f;
	if(diffuseCoefficient > 0.0f) {
		//0.06 number is shininess
		specularCoefficient = pow(max(0.0f, dot(surfaceToCamera.xyz, reflect(-currLight.direction.xyz, normal.xyz))), 10.0f);
	}
	//shininess * coeff * colour * intensity
	vec3 specular = 0.03f * specularCoefficient * vec3(1,1,1) * currLight.intensities.xyz;

	return ambient + attenuation * (diffuse + specular);
}

void main() {

	//set initial fragment colour
	vec4 tempFragColour = texture(texSampler, texCoord);

	//Get direction between camera and surface (Used for specular effect)
	vec3 surfaceToCam = normalize(camPos.xyz
	 - vertexPos.xyz);

	vec3 totalLightColour = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < numLights; i++) {

		totalLightColour += applyLight(lights[i], vertexPos.xyz, tempFragColour.xyz, surfaceToCam);
	}

	vec3 gamma = vec3(1.0/2.2);

	fragColour = vec4(pow(totalLightColour, gamma), tempFragColour.a);
}