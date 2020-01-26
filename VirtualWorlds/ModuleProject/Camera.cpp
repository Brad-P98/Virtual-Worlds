#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 targetPos)
{
	position = startPosition;
	front = glm::normalize(targetPos);

	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
	up = glm::cross(front, right);

	viewMat = glm::mat4(1.0);
	viewMat = glm::lookAt(position, front, up);
}


Camera::~Camera()
{
}


void Camera::update() {

	prevPosition = position;
	xDelta = 0.0f;
	yDelta = 0.0f;


	checkMove();
	checkRotate();



}

void Camera::checkMove()
{
	if (InputHandler::checkKeyPressed('w')) {
		position += maxSpeed * front * (float)Clock::deltaTime;
	}
	if (InputHandler::checkKeyPressed('s')) {
		position -= maxSpeed * front * (float)Clock::deltaTime;
	}
	if (InputHandler::checkKeyPressed('a')) {
		position -= glm::normalize(glm::cross(front, up)) * maxSpeed * (float)Clock::deltaTime;
	}
	if (InputHandler::checkKeyPressed('d')) {
		position += glm::normalize(glm::cross(front, up)) * maxSpeed * (float)Clock::deltaTime;
	}
	viewMat = glm::translate(viewMat, position - prevPosition);
}

void Camera::checkRotate()
{
	if (InputHandler::checkMousePressed(2)) {
		xDelta = InputHandler::mouseDelta.x;
		yDelta = InputHandler::mouseDelta.y;

		yaw += xDelta * xSens * Clock::deltaTime;
		//std::cout << yaw << std::endl;
		pitch -= yDelta * ySens * Clock::deltaTime;
		//std::cout << pitch << std::endl;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	viewMat = glm::lookAt(position, position + front, up);
}
