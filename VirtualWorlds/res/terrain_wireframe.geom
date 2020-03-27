#version 460

mat4 mvpMatrix;

in vec3[] normal;
in float[] score;

out float score2;

layout (triangles) in;

layout(line_strip, max_vertices = 6) out;

layout (std140) uniform Camera
{
	mat4 projection;
	mat4 view;
    vec4 camPos;
};

layout (std140) uniform Model
{
	mat4 model;
};

void main(void) {

    mvpMatrix = projection * view * model;

    gl_Position = mvpMatrix * gl_in[0].gl_Position;
    score2 = score[0];
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[1].gl_Position;
    score2 = score[1];
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[2].gl_Position;
    score2 = score[2];
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[0].gl_Position;
    score2 = score[0];
    EmitVertex();

    EndPrimitive();

    //Normal
    // gl_Position = mvpMatrix * gl_in[0].gl_Position;
    // EmitVertex();

    // vec4 normalPoint = gl_in[0].gl_Position + (vec4(normal[0] ,0)) * score[0];
    // gl_Position = mvpMatrix * normalPoint;
    // EmitVertex();

    //EndPrimitive();
}