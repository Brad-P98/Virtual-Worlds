#pragma once
#include <glm/glm/glm.hpp>

struct DirectionalLight {
	glm::vec4 direction = glm::vec4(0.0f);
	glm::vec4 colourIntensities;
	float ambient;
	float pad1;
	float pad2;
	float pad3;
};

