#version 330 core

layout(location = 0) in vec3 vertexPosIn;
layout(location = 2) in vec3 normalIn;
layout(location = 4) in vec2 texCoordIn;

out vec3 vertexPos;
out vec3 normal;
out vec4 camPos;
out vec2 texCoord;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec4 camPosIn;
};

layout (std140) uniform Model
{
	mat4 model;
};


void main() {

	//world space vertex passed to frag
	vertexPos = (model * vec4(vertexPosIn, 1.0f)).xyz;

	normal = normalize(normalIn);

	//cam vertexPosIn passed to frag
	camPos = camPosIn;
	
	//Tex coord passed straight to frag 
	texCoord = texCoordIn;

	gl_Position = projection * view * model * vec4(vertexPosIn, 1.0f);
}