#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 targetPos)
{
	position = startPosition;
	front = glm::normalize(targetPos);

	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
	up = glm::cross(front, right);

	projectionMat = glm::perspective(glm::radians(60.0f), 800.0f / 640.0f, 0.1f, 100.0f);

	viewMat = glm::mat4(1.0);
	viewMat = glm::lookAt(position, front, up);

}


Camera::~Camera()
{
}

//Initialize UBOs for the camera view and projection matrices
void Camera::initUBOs(GLuint shader)
{

	unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader, "Camera");
	glUniformBlockBinding(shader, uniformBlockIndex, 0);

	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboCamera, 0, 2 * sizeof(glm::mat4));
	
	//Setup the projection matrix part of the ubo
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projectionMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}


void Camera::update() {

	prevPosition = position;
	xDelta = 0.0f;
	yDelta = 0.0f;

	checkMove();
	checkRotate();

	//Now update the view matrix part of the ubo with the new view matrix
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

void Camera::updateUBOCamera()
{

	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
