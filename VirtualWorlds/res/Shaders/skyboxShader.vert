#version 330 core

layout(location = 0) in vec3 posIn;

out vec3 texCoords;


layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
	vec4 camPosIn;
};

void main() {
    texCoords = posIn;
    mat4 v = view;
    v[3][0] = 0.0f;
    v[3][1] = 0.0f;
    v[3][2] = 0.0f;
    vec4 pos = projection *v * vec4(100 * posIn, 1.0);
    gl_Position = pos.xyww;
}