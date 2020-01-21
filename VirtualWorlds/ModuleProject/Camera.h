#pragma once
#include <glm/glm/glm.hpp>

#include "Clock.h"
#include "InputHandler.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startDirection);
	~Camera();

	void update();

	glm::vec3 position;
	glm::vec3 direction;

	//Vector pointing directly to the right of the camera
	glm::vec3 right;
	glm::vec3 up;

private:

	float maxSpeed = 2.0f;
	float currentSpeed = 0.0f;

};

