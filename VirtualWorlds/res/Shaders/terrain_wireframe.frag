#version 460

out vec4 fragColour;

in float score2;

float maxScore = 100;

void main() {

    vec4 tempFragColour = vec4(0.0, 0.0, 1.0, 1.0);

    vec3 red = vec3(1,0,0);
    vec3 green = vec3(0,1,0);

    vec3 col = mix(red, green, score2/maxScore);

    fragColour = vec4(col, 1.0);
}