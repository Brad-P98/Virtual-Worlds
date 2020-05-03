#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm/gtx/rotate_vector.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Clock.h"
#include "InputHandler.h"
#include "ShaderManager.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startDirection);
	~Camera();

	void initCameraUBOs();

	void update();

	glm::mat4 getProjMatrix() const { return projectionMat; }
	glm::mat4 getViewMatrix() const { return viewMat; }

	glm::vec3 getWorldPos() const { return position; }
	float getNearPlane() const { return nearPlane; }
	float getFarPlane() const { return farPlane; }

	void setWorldPos(glm::vec3 pos) {
		position = pos;
	}

private:

	void checkMove();
	void checkRotate();

	void updateCameraUBOs();

private:

	struct CameraBuffer {
		glm::mat4 projMat;
		glm::mat4 viewMat;
		glm::vec4 camPos;
		float nearPlane;
		float farPlane;
	} cameraBuffer;

	glm::mat4 projectionMat;
	glm::mat4 viewMat;
	unsigned int cameraUBO;

	glm::vec3 position;
	glm::vec3 prevPosition;

	//Normalized direction vectors for camera axes
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float nearPlane = 1.0f;
	float farPlane = 5000.0f;

private:
	
	float maxSpeed = 2009.0f;
	float currentSpeed = 0.0f;

	float xSens = 0.03f;
	float ySens = 0.03f;

	float xDelta = 0.0f;	//rotation amount on x axis (pitch)
	float yDelta = 0.0f;	//yaw

	float pitch = 0.0f;	//Total pitch
	float yaw = -90.0f;		//Total yaw
};

