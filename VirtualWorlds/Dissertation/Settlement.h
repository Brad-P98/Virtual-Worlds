#pragma once
#include <glm/glm/glm.hpp>

#include "Building.h"
#include "NoiseGenerator.h"
#include "RoadSegment.h"
#include "PathNodes.h"

class Settlement
{
public:

	Settlement(glm::vec3 focalPos, float score);
	~Settlement();

	void constructSettlement();
	void destroySettlement();


private:

	void constructStreets();
	void constructBuildings();

	glm::vec3 sampleSurroundingPointsLowestElevationChange(glm::vec3 startPoint, int numSamples, float sampleRadius);

	//Construct a single street
	void constructStreet(glm::vec3 startPos, glm::vec3 endPos);

	void constructOffshootRoads();

public:

	//A settlement with a high influence score is larger. With a high score, nearby settlements are smaller. 
	float influenceScore;

	int settlementID;

	//'Center' of the settlement
	glm::vec3 focalPointPos;

	//grid coordinates of the chunk that this settlement resides on
	glm::vec3 homeChunkCoords;



	NoiseGenerator* terrainNoise;
	NoiseGenerator* myNoise;

private:
	//ROAD SETUP
	//Store the end of roads on the outside of the settlement that other settlement roads may connect to
	std::vector<glm::vec3> roadConnectionPoints;

	//Store positions of every street start/end point in the grid.
	std::vector<std::vector<glm::vec3>> streetLayoutGrid;

	std::vector<glm::vec3> buildingPositions;

	float gridSegWidth = 14.0f;
	float gridSegLength = 22.0f;

	std::vector<Building*> myBuildings;
	std::vector<RoadSegment*> myRoads;

	HLPathNode* myPathNode;
};

