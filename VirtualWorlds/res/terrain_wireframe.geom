#version 460

uniform mat4 mvpMatrix;

layout (triangles) in;

layout(line_strip, max_vertices = 4) out;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
};

layout (std140) uniform Model
{
	mat4 model;
};

void main(void) {

    gl_Position = projection * view * model * gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = projection * view * model * gl_in[1].gl_Position;
    EmitVertex();

    gl_Position = projection * view * model * gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = projection * view * model * gl_in[0].gl_Position;
    EmitVertex();


    EndPrimitive();
}