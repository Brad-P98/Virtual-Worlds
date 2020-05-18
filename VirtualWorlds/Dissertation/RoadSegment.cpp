#include "RoadSegment.h"
#include <VAOLoader.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Instance.h>

#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtx/quaternion.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

RoadSegment::RoadSegment(glm::vec3 startPos, glm::vec3 endPos)
{
	//Calculate length of road
	myLength = calcDist(startPos, endPos);
	float lengthModifier = myLength / 8.9f;

	//Find position in world to place the object.
	glm::vec3 midPoint = getMidpoint(startPos, endPos);

	//Calculate the rotation amount needed for each axis(radians)
	glm::vec3 rot = calcRotation(startPos, endPos);


	transform = glm::translate(glm::mat4(1), midPoint + glm::vec3(0,0.5f,0));

	transform = glm::rotate(transform, rot.y, glm::vec3(0, 1, 0));
	transform = glm::rotate(transform, rot.x, glm::vec3(0, 0, 1));

	transform = glm::scale(transform, glm::vec3(0.15 * lengthModifier, 0.15, 0.15));

	m_StartPos = startPos;
	m_EndPos = endPos;


	thisInstance = new StaticInstance();
	//If instance of this object exists, add instance.
	if (Instance::m_scene->instancedObjectExists("RoadSegment")) {

		thisInstance->modelMat = transform;
		Instance::m_scene->getInstancedObject("RoadSegment")->addInstance(thisInstance);
	}
	else {
		//Create new object to contain instances
		std::vector<GLuint> texIDs;
		texIDs.push_back(TextureManager::getTextureID("Assets/road.jpg"));

		//Create new instanced object, and give it Building's properties.
		InstancedObject3D* newInstancedObject = new InstancedObject3D();
		newInstancedObject->init(VAOLoader::getModelVAO("road.obj"), texIDs, ShaderManager::getShader("instanced_basic"));

		Instance::m_scene->addInstancedObject("RoadSegment", newInstancedObject);

		//Create and add first static instance
		thisInstance->modelMat = transform;
		Instance::m_scene->getInstancedObject("RoadSegment")->addInstance(thisInstance);
	}


}

RoadSegment::~RoadSegment()
{
	Instance::m_scene->getInstancedObject("RoadSegment")->removeInstance(thisInstance);
}


float RoadSegment::calcDist(glm::vec3 startPosition, glm::vec3 endPosition)
{
	float xDist = abs(startPosition.x - endPosition.x);
	float yDist = abs(startPosition.y - endPosition.y);
	float zDist = abs(startPosition.z - endPosition.z);

	float res = sqrt((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
	return res;
}

glm::vec3 RoadSegment::getMidpoint(glm::vec3 startPosition, glm::vec3 endPosition)
{
	float xMid = (startPosition.x + endPosition.x) / 2;
	float yMid = (startPosition.y + endPosition.y) / 2;
	float zMid = (startPosition.z + endPosition.z) / 2;
	glm::vec3 midpoint = glm::vec3(xMid, yMid, zMid);
	return midpoint;
}

glm::vec3 RoadSegment::calcRotation(glm::vec3 startPosition, glm::vec3 endPosition)
{
	glm::vec3 rotations = glm::vec3(0.0f);
	float xDist = endPosition.x - startPosition.x;
	float yDist = endPosition.y - startPosition.y;
	float zDist = endPosition.z - startPosition.z;


	rotations.y = M_PI - atan(zDist / xDist);

	if(xDist != 0) rotations.x = M_PI - atan(yDist  *0.5f / xDist);
	else rotations.x = M_PI - atan(yDist *0.5f / zDist);


	return rotations;
}
