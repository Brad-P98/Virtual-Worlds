#version 460

layout(location = 0) in position;

void main(void) {
    gl_position = position;
}