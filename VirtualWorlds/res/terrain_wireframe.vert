#version 460

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normalIn;

out vec3 normal;

void main(void) {
    
    gl_Position = vec4(position, 1.0f);
    normal = normalIn;
}