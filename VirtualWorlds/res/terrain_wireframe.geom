#version 460

mat4 mvpMatrix;

in vec3[] normal;

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
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[1].gl_Position;;
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[2].gl_Position;
    EmitVertex();

    gl_Position = mvpMatrix * gl_in[0].gl_Position;
    EmitVertex();

    EndPrimitive();

    //Normal
    gl_Position = mvpMatrix * gl_in[0].gl_Position;
    EmitVertex();

    vec4 normalPoint = gl_in[0].gl_Position - vec4(normal[0] ,0);
    gl_Position = mvpMatrix * normalPoint;
    EmitVertex();

    EndPrimitive();
}