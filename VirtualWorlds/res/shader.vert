#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 2) in vec3 normalIn;

out vec4 vertexColour;
out vec3 normal;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};

layout (std140) uniform Model
{
	mat4 model;
};


void main() {


	gl_Position = projection * view * model * vec4(pos, 1.0f);
	vertexColour = vec4(0.565, 0.933, 0.565, 1.0);
	normal = normalize(normalIn);
}