#pragma once
#include <glm/glm/glm.hpp>

struct DirectionalLight {
	glm::vec3 direction = glm::vec3(0.0f);
	glm::vec3 colourIntensities;
	float ambient;
};

