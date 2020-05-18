#version 330 core

layout(location = 0) in vec3 vertexPosIn;
layout(location = 2) in vec3 normalIn;
layout(location = 4) in vec2 texCoordIn;

out vec3 vertexPos;
out vec3 normal;
out vec4 camPos;
out vec2 texCoord;
out float visibility;

//Fog settings
const float density = 0.0008;
const float gradient = 4.0;

//Texture tiling factor. temp
float tilingFactor = 7.0;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec4 camPosIn;
	float nearPlane;
	float farPlane;
};

layout (std140) uniform Model
{
	mat4 model;
};


void main() {

	//world space vertex passed to frag
	vertexPos = (model * vec4(vertexPosIn, 1.0f)).xyz;

	//fog calculation
	vec4 posRelToCam = vec4(view * vec4(vertexPos, 1.0));
	float distance = length(posRelToCam.xyz);
	visibility = exp(-pow((distance * density),gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	normal = normalize(normalIn);

	//cam vertexPosIn passed to frag
	camPos = camPosIn;
	
	//Tex coord passed straight to frag 
	texCoord = texCoordIn * tilingFactor;

	gl_Position = projection * view * model * vec4(vertexPosIn, 1.0f);
}