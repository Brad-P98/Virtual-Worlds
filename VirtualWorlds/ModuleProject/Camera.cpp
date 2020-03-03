#include "Camera.h"



Camera::Camera(glm::vec3 startPosition, glm::vec3 startDirection)
{
	position = startPosition;

	yaw = glm::degrees(atan2(startDirection.x, startDirection.z));
	pitch = 0;

	front = glm::normalize(startDirection);

	right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
	up = glm::cross(front, right);

	projectionMat = glm::perspective(glm::radians(60.0f), 800.0f / 640.0f, 0.1f, 1000.0f);

	viewMat = glm::mat4(1.0);
	viewMat = glm::lookAt(position, position + front, up);

}


Camera::~Camera()
{
}

//Initialize UBOs for the camera view and projection matrices
void Camera::initCameraUBOs()
{
	//iterate through all shader programs
	for (auto shader : ShaderManager::shaderPrograms) {

		//If shader requires camera view and proj, set up ubo
		if (glGetUniformBlockIndex(shader.second, "Camera") != GL_INVALID_INDEX) {
			unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.second, "Camera");
			glUniformBlockBinding(shader.second, uniformBlockIndex, 0);

			glGenBuffers(1, &cameraUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, cameraUBO, 0, sizeof(cameraBuffer));
		}
	}
	updateCameraUBOs();
}


void Camera::update() {

	prevPosition = position;
	xDelta = 0.0f;
	yDelta = 0.0f;

	checkMove();
	checkRotate();

	//Now update the UBO view matrix and camera position
	updateCameraUBOs();
}

void Camera::checkMove()
{
	if (InputHandler::checkKeyPressed('\\')) {
		maxSpeed = 0.012f;
	}
	else maxSpeed = 0.04f;

	if (InputHandler::checkKeyPressed('w') || InputHandler::checkKeyPressed('W')) {
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

	if (InputHandler::checkKeyPressed('r')) {
		position += maxSpeed * up * (float)Clock::deltaTime;
	}
	if (InputHandler::checkKeyPressed('f')) {
		position -= maxSpeed * up * (float)Clock::deltaTime;
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

		//front = glm::rotate(front, yaw, glm::vec3(0, 1, 0));
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(direction);

	viewMat = glm::lookAt(position, position + front, up);
}

void Camera::updateCameraUBOs()
{
	cameraBuffer.projMat = projectionMat;
	cameraBuffer.viewMat = viewMat;
	cameraBuffer.camPos = glm::vec4(getWorldPos(), 1.0f);

	glBindBuffer(GL_UNIFORM_BUFFER, cameraUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(cameraBuffer), &cameraBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
