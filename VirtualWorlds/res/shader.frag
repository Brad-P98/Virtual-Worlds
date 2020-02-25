#version 330 core

out vec4 fragColour;

in vec4 vertexColour;

in vec3 pos2;
in vec3 normal;

void main() {

	float mod = dot(normal, vec3(0,1,0));

	fragColour = clamp(mod, 0.4, 1.0) * vertexColour;
}