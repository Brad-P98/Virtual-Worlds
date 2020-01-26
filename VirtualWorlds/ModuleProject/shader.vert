#version 330 core

layout(location = 0) in vec3 pos;

out vec4 vertexColour;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

void main() {
	gl_Position = projection * view * model * vec4(pos, 1.0f);
	vertexColour = vec4(0.5, 0.0, 0.0, 1.0);
}