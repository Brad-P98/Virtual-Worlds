#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec3 normalIn;

out vec3 vertexPos;
out vec4 vertexColour;
out vec3 normal;
out vec4 camPos;

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
	vertexPos = (model * vec4(pos, 1.0f)).xyz;
	//temp vertex colour passed to frag
	vertexColour = vec4(0.565, 0.933, 0.565, 1.0);
	normal = normalize(normalIn);
	//cam pos passed to frag
	camPos = camPosIn;

	gl_Position = projection * view * model * vec4(pos, 1.0f);
}