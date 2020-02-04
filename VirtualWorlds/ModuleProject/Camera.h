#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Clock.h"
#include "InputHandler.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startDirection);
	~Camera();

	void initUBOs(GLuint shader);

	void update();

	glm::mat4 getProjMatrix() { return projectionMat; }
	glm::mat4 getViewMatrix() { return viewMat; }

	glm::vec3 getWorldPos() const
	{
		return position;
	}

private:

	void checkMove();
	void checkRotate();

	void updateUBOCamera();

private:

	glm::mat4 projectionMat;
	glm::mat4 viewMat;
	unsigned int cameraUBO;

	glm::vec3 position;
	glm::vec3 prevPosition;

	//Normalized direction vectors for camera axes
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

private:
	
	float maxSpeed = 0.004f;
	float currentSpeed = 0.0f;

	float xSens = 0.03f;
	float ySens = 0.03f;

	float xDelta = 0.0f;	//rotation amount on x axis (pitch)
	float yDelta = 0.0f;	//yaw

	float pitch = 0.0f;	//Total pitch
	float yaw = -90.0f;		//Total yaw
};

