#pragma once
#include <InstancedObject3D.h>


class RoadSegment
{
public:
	RoadSegment(glm::vec3 startPosition, glm::vec3 endPosition);
	~RoadSegment();

public:

	glm::vec3 m_StartPos;
	glm::vec3 m_EndPos;

	//Store the IDs of the nodes that this segment helps connect.
	std::pair<int, int> HLNodeIDs;

private:

	float calcDist(glm::vec3 startPosition, glm::vec3 endPosition);
	glm::vec3 getMidpoint(glm::vec3 startPosition, glm::vec3 endPosition);

	glm::vec3 calcRotation(glm::vec3 startPosition, glm::vec3 endPosition);

private:




	glm::mat4 transform;
	StaticInstance* thisInstance;


	//Length of each segment is 8.9 world units with length 1.0f
	const float length = 1.0f;


	float myLength;	//In world units

};

