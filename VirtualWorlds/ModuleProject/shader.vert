#version 330 core

layout(location = 0) in vec3 pos;

out vec4 vertexColour;


uniform mat4 modelTrans;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * modelTrans * vec4(pos, 1.0f);
	vertexColour = vec4(0.5, 0.0, 0.0, 1.0);
}