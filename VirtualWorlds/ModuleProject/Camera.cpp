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

	if (InputHandler::checkKeyPressed('s') || InputHandler::checkKeyPressed('S')) {
		//backward
		position.z -= position.z * maxSpeed * Clock::deltaTime;
	}
}