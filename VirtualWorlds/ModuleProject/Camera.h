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

	float xDelta;	//rotation amount on x axis (pitch)
	float yDelta;	//yaw

private:

	float maxSpeed = 0.002f;
	float currentSpeed = 0.0f;

	float xSens = 0.1f;
	float ySens = 0.1f;

	float rotSpeed = 1.0f;



};

