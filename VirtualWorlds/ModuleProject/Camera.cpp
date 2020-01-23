#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startDirection)
{
	position = startPosition;
	direction = startDirection;

	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction));
	up = glm::cross(direction, right);

}


Camera::~Camera()
{
}


void Camera::update() {

	xDelta = 0;
	yDelta = 0;

	//Handle movement
	if (InputHandler::checkKeyPressed('s') || InputHandler::checkKeyPressed('S')) {
		//backward
		position.z -= maxSpeed * Clock::deltaTime;
		std::cout << position.z << std::endl;
	}

	//Handle looking
	//If right mouse btn clicked
	if (InputHandler::checkMousePressed(2)) {
		xDelta = InputHandler::mouseDelta.y * xSens * Clock::deltaTime;
		yDelta = InputHandler::mouseDelta.x * ySens * Clock::deltaTime;
	}
}