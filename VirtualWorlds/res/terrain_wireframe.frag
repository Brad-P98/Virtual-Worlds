#version 460

layout(location = 0) out vec4 fragColour;

void main(void) {
    fragColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    gl_FragDepth = gl_FragCoord.z * 0.999f;
}