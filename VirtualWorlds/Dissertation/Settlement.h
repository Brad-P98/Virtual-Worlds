#pragma once
#include <glm/glm/glm.hpp>


class Settlement
{
public:

	Settlement();
	~Settlement();

	void generateSettlement();
public:

	//A settlement with a high influence score is larger. With a high score, nearby settlements are smaller. 
	float influenceScore;


	//'Center' of the settlement
	glm::vec3 focalPointPos;

	//grid coordinates of the chunk that this settlement resides on
	glm::vec2 homeChunkCoords;

private:

	//Vector of buildings game objects.
};

