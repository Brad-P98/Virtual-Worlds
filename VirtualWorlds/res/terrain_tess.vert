#version 460

layout(location = 0) in vec3 vertexPosIn;
layout(location = 2) in vec3 normalIn;
layout(location = 4) in vec2 texCoordIn;

//Fog settings
const float density = 0.001;
const float gradient = 3.0;

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


out ControlPoint {

	float linearDepth;
	vec3 normal;
	vec4 worldPos;
	vec2 textureCoord;
	vec4 camPos;
	float visibility;

} outputControlPoint;

void main() {

	//fog calculation
	vec4 posRelToCam = vec4(view * vec4(vertexPosIn, 1.0));
	float distance = length(posRelToCam.xyz);
	outputControlPoint.visibility = exp(-pow((distance * density),gradient));
	outputControlPoint.visibility = clamp(outputControlPoint.visibility, 0.0, 1.0);


	vec4 eyePos = view * model * vec4(vertexPosIn, 1.0f);
	outputControlPoint.linearDepth = (-eyePos.z - nearPlane) / (farPlane - nearPlane);

	outputControlPoint.normal = normalIn;
	outputControlPoint.worldPos = vec4(vertexPosIn, 1.0f);
	outputControlPoint.textureCoord = texCoordIn * tilingFactor;
	outputControlPoint.camPos = camPosIn;

	gl_Position = eyePos;
}