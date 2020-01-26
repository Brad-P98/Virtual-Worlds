#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "VAOData.h"
#include "Renderer.h"

class GameObject {

	virtual void update() = 0;

	virtual void draw() = 0;
};
