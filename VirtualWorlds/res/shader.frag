#version 330 core

out vec4 fragColour;

in vec4 vertexColour;

in vec3 normal;

struct Light {
	vec3 direction;
	vec3 intensities;
	float ambient;
};

#define MAX_LIGHTS 20
layout (std140) uniform Lights 
{

	Light lights[MAX_LIGHTS];
	int numLights;
};

void main() {

	float mod = max(dot(normal, vec3(0,1,0)), 0);
	mod += lights[0].ambient;
	//mod += 0.2f;
	fragColour = mod * vertexColour;
}