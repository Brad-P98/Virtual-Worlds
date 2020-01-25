#pragma once
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Clock.h"
#include "InputHandler.h"

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startDirection);
	~Camera();

	void update();

	glm::mat4 getViewMatrix() { return viewMat; }


	glm::mat4 viewMat;

	glm::vec3 position;
	glm::vec3 prevPosition;

	//Normalized direction vectors for camera axes
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	float xDelta = 0.0f;	//rotation amount on x axis (pitch)
	float yDelta = 0.0f;	//yaw

	float pitch = 0.0f;	//Total pitch
	float yaw = -90.0f;		//Total yaw

private:

	float maxSpeed = 0.004f;
	float currentSpeed = 0.0f;

	float xSens = 0.03f;
	float ySens = 0.03f;

	float rotSpeed = 1.0f;

	void checkMove();
	void checkRotate();

	void outVec3(glm::vec3 vec)
	{
		std::cout << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z << std::endl;
	}

};

