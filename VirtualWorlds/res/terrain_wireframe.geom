#version 460

uniform mat4 mvpMatrix;

layout (triangles) in;

layout(line_strip, max_vertices = 4) out;

void main(void) {

    gl_position = mvpMatrix * gl_in[0].gl_position;
    EmitVertex();

    gl_position = mvpMatrix * gl_in[1].gl_position;
    EmitVertex();

    gl_position = mvpMatrix * gl_in[2].gl_position;
    EmitVertex();

    gl_position = mvpMatrix * gl_in[0].gl_position;
    EmitVertex();


    EndPrimitive();
}