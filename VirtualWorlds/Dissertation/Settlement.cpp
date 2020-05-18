#include "Settlement.h"

#include "ChunkSettings.h"
#include <Instance.h>

#include "SettlementManager.h"
#include "RoadNetwork.h"

#define _USE_MATH_DEFINES
#include <math.h>

Settlement::Settlement(glm::vec3 focalPos, float score) : influenceScore(score)
{
	focalPointPos = focalPos + glm::vec3(0, 1, 0);

	homeChunkCoords = glm::vec3(round(focalPos.x / ChunkSettings::CHUNK_SIZE + 0.499f) - 1, 0, round(focalPos.z / ChunkSettings::CHUNK_SIZE + 0.499f) - 1);

	myNoise = new NoiseGenerator();
	myNoise->noiseGenerator = new PerlinNoise();
	myNoise->layers.push_back(new NoiseLayer(4, 0.03f));	//Always from -4 to 4

	terrainNoise = SettlementManager::getInstance()->terrainNoiseGenerator;

}

Settlement::~Settlement()
{
	destroySettlement();
}

void Settlement::constructSettlement()
{	
	
	//Add to network to gather neighbours
	RoadNetwork::getInstance()->addNode(this);
	myPathNode = RoadNetwork::getInstance()->getHLNode(this);


	//Construct the random-ish road segments for the settlement
	//create a bunch of 'road endings' positions on the outside of the settlement, that the road pathfinder can start from, and looks for on other settlements
	//This method also adds building spots to the sides of the roads
	constructStreets();

	//Now, link this node with other settlements
	RoadNetwork::getInstance()->linkNode(myPathNode);

}

void printVec3(std::string s, glm::vec3 vec3)
{
	std::cout << s << ": " << vec3.x << ",  " << vec3.y << ", " << vec3.z << std::endl;
}

void Settlement::constructStreets()
{
	std::vector<glm::vec3> buildingPositions;


	//Create streets based on terrain. 
	//Sample some surrounding points, and find least elevation change. Build roads, add points for buildings along roads
	
	//STEP 1. Work out the 'direction' that the grid will be in.

	//Get the point from circle around focal point where elevation change is least.
	glm::vec3 lowestElevChangePoint = sampleSurroundingPointsLowestElevationChange(focalPointPos, 6, 40);

	//Grid Direction based off this. Only considers XZ
	glm::vec3 gridDirLengthways = glm::normalize(focalPointPos - lowestElevChangePoint);
	gridDirLengthways.y = 0;

	glm::vec3 gridDirCross = glm::normalize(glm::cross(gridDirLengthways, glm::vec3(0, 1, 0)));


	//STEP 2. Work out the grid size based on influence score
	int gridXDim = 2;	//Default small
	int gridZDim = 2;	//Default small
	if (influenceScore > 150) { gridXDim = 3; gridZDim = 3; }
	else if (influenceScore >= 100) { gridXDim = 2; gridZDim = 2; }
	else {}	//Stays as default

	streetLayoutGrid.resize(gridXDim * 2 + 1);

	//STEP 3. Generate grid of points based on the above factors
	for (int i = -gridXDim; i < gridXDim + 1; i++) {	//Range -3 to 4, for a 7-wide grid
		for (int j = -gridZDim; j < gridZDim + 1; j++) {

			glm::vec3 p = focalPointPos + (i * gridSegWidth*gridDirCross) + (j * gridSegLength*gridDirLengthways);

			p.y = terrainNoise->generateTotalNoise(p.x, p.z);
			//Building* b = new Building(p, 1);
			streetLayoutGrid[i + gridXDim].push_back(p);
		}
	}

	//STEP 4. Generate the street grid
	//Iterate through
	for (int i = 0; i < streetLayoutGrid.size(); i++) {
		for (int j = 0; j < streetLayoutGrid[i].size() - 1; j++) {

			//If any of the nodes are underwater, do not build street
			if (streetLayoutGrid[i][j].y < ChunkSettings::SEA_LEVEL || streetLayoutGrid[i][j + 1].y < ChunkSettings::SEA_LEVEL) {
				//Skip over this one
				continue;
			}
			else {
				//Create road segment between this j and next j
				constructStreet(streetLayoutGrid[i][j], streetLayoutGrid[i][j + 1]);
			}


		}
	}
	for (int i = 0; i < streetLayoutGrid.size() - 1; i++) {
		for (int j = 0; j < streetLayoutGrid[i].size(); j++) {

			if (streetLayoutGrid[i][j].y < ChunkSettings::SEA_LEVEL || streetLayoutGrid[i + 1][j].y < ChunkSettings::SEA_LEVEL) 
			{
				continue;
			}
			else {
				//Create road segment between this j and next j
				constructStreet(streetLayoutGrid[i][j], streetLayoutGrid[i + 1][j]);
			}


		}
	}

	constructBuildings();


	//STEP 5. Calculate which outside grid positions should have an offshoot road that other settlements can connect to
	constructOffshootRoads();

}

void Settlement::constructBuildings()
{

	for (int i = 0; i < buildingPositions.size(); i++) {

		float myHMod = myNoise->generateTotalNoise(buildingPositions[i].x, buildingPositions[i].z);
		if (myHMod < 1) myHMod = 1;
		Building* newB = new Building(buildingPositions[i], myHMod);
		myBuildings.push_back(newB);
	}
}

glm::vec3 Settlement::sampleSurroundingPointsLowestElevationChange(glm::vec3 startPoint, int numSamples, float sampleRadius)
{
	float angleStep = (2 * M_PI) / numSamples;
	glm::vec3 bestPoint = glm::vec3(0,0,0);
	float bestElevChange = INFINITY;
	for (int i = 0; i < numSamples; i++) {
		glm::vec3 samplePoint = glm::vec3(startPoint.x + sampleRadius * cos(angleStep*i), startPoint.y, startPoint.z + sampleRadius * sin(angleStep*i));

		samplePoint.y = terrainNoise->generateTotalNoise(samplePoint.x, samplePoint.z);
		float elevChange = abs(samplePoint.y - startPoint.y);
		if (elevChange < bestElevChange && samplePoint.y > ChunkSettings::SEA_LEVEL) {
			bestPoint = samplePoint;
			bestElevChange = elevChange;
		}
	}

	return bestPoint;
}

void Settlement::constructStreet(glm::vec3 startPos, glm::vec3 endPos)
{
	RoadSegment* newStreet = new RoadSegment(startPos, endPos);
	myRoads.push_back(newStreet);

	float buildingSpacing = 4.0f;	//Spacing between buildings on street

	//Establish the number of houses buildable on each side of the road
	float streetLength = glm::distance(startPos, endPos);
	int numSpaces = round(streetLength / buildingSpacing);

	//Direction, and perpendicular direction vectors
	glm::vec3 roadDir = glm::normalize(newStreet->m_StartPos - newStreet->m_EndPos);
	glm::vec3 roadDirCross = glm::normalize(glm::cross(newStreet->m_StartPos - newStreet->m_EndPos, glm::vec3(0, 1, 0)));

	//Calculate building positions
	for (int i = 1; i < numSpaces; i++) {

		glm::vec3 posOnRoad = newStreet->m_StartPos - (roadDir * glm::vec3(i*buildingSpacing, i*buildingSpacing, i*buildingSpacing));

		glm::vec3 posOnRoadSideP = posOnRoad + (roadDirCross * glm::vec3(3.9f, 0, 3.9f));
		glm::vec3 posOnRoadSideN = posOnRoad - (roadDirCross * glm::vec3(3.9f, 0, 3.9f));
		posOnRoadSideP.y = terrainNoise->generateTotalNoise(posOnRoadSideP.x, posOnRoadSideP.z);
		posOnRoadSideN.y = terrainNoise->generateTotalNoise(posOnRoadSideN.x, posOnRoadSideN.z);
		//Add positions of road ends to vector, and remove those that are now not end roads.
		if(posOnRoadSideP.y > ChunkSettings::SEA_LEVEL) buildingPositions.push_back(posOnRoadSideP);
		if(posOnRoadSideN.y > ChunkSettings::SEA_LEVEL) buildingPositions.push_back(posOnRoadSideN);
	}

}

void Settlement::constructOffshootRoads()
{
	//Grab all 'outside' road intersection points.
	std::vector<glm::vec3> outsidePositions;
	for (int i = 0; i < streetLayoutGrid.size(); i++) {
		for (int j = 0; j < streetLayoutGrid[i].size(); j++) {

			//If either index is the first or last in their vector, its an 'outside' position.
			if (i == 0 || i == streetLayoutGrid.size() - 1) {
				outsidePositions.push_back(streetLayoutGrid[i][j]);
			}
			else if (j == 0 || j == streetLayoutGrid.size() - 1) {
				outsidePositions.push_back(streetLayoutGrid[i][j]);
			}
			else continue;
		}
	}

	//Construct off shoot road at intervals
	for (int i = 0; i < outsidePositions.size(); i += 5) {
		float offshootLength = 7.0f;
		glm::vec3 start = outsidePositions[i];
		//Direction of offshoot is in opp direction of focal pos
		glm::vec3 dir = glm::normalize(outsidePositions[i] - focalPointPos);
		glm::vec3 end = start + dir * offshootLength;
		end.y = terrainNoise->generateTotalNoise(end.x, end.z);
		RoadSegment* offshoot = new RoadSegment(start, end);
		myRoads.push_back(offshoot);

		//Add the connection spot to the node's starting points.
		myPathNode->connectingPoints.push_back(end);
	}

}



void Settlement::destroySettlement()
{
	for (int i = 0; i < myBuildings.size(); i++) {
		delete myBuildings[i];
		myBuildings.clear();
	}

	RoadNetwork::getInstance()->removeNode(this);

	for (int i = 0; i < myRoads.size(); i++) {
		delete myRoads[i];
		myRoads.clear();
	}
}


