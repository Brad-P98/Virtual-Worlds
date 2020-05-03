#version 330 core

//OUT
out vec4 fragColour;

//IN
in vec3 vertexPos;
in vec3 normal;
in vec4 camPos;
in vec2 texCoord;
in float visibility;

uniform sampler2D texSampler0;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform sampler2D texSampler3;
uniform sampler2D texSampler4;

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
	vec3 specular = 0.015f * specularCoefficient * vec3(1,1,1) * currLight.intensities.xyz;

	return ambient + attenuation * (diffuse + specular);
}


void main() {



	//set initial fragment colour
	vec4 tempFragColour = texture(texSampler2, texCoord);

	//Calculate color at this height.
	//Sample colour from each texture
	vec4 grassColour = texture(texSampler0, texCoord);
	vec4 snowColour = texture(texSampler1, texCoord);
	vec4 gravelColour = texture(texSampler2, texCoord);

	float height = vertexPos.y;
	float mixAmount = 0.0;

	//Grassy
	mixAmount = clamp(pow(height + 21, 2) / 20, 0, 1);
	tempFragColour = mix(texture(texSampler2, texCoord), texture(texSampler0, texCoord), mixAmount);

	//Snowy
	mixAmount = clamp(pow(height - 30, 1.6) /30, 0, 1);
	tempFragColour = mix(tempFragColour, texture(texSampler1, texCoord), mixAmount);


	//Lighting
	//Get direction between camera and surface (Used for specular effect)
	vec3 surfaceToCam = normalize(camPos.xyz
	 - vertexPos.xyz);

	vec3 totalLightColour = vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < numLights; i++) {

		totalLightColour += applyLight(lights[i], vertexPos.xyz, tempFragColour.xyz, surfaceToCam);
	}

	vec3 gamma = vec3(1.0/2.2);

	//lighting
	fragColour = vec4(pow(totalLightColour, gamma), tempFragColour.a);
	//fog
	fragColour = mix(vec4(0.5,0.7,0.8,1.0), fragColour, visibility);
}