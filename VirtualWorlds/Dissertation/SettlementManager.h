#pragma once
#include <vector>
#include <map>
#include <glm/glm/glm.hpp>

#include "Settlement.h"

class SettlementManager
{
private:

	static SettlementManager* instance;

public:
	SettlementManager();
	~SettlementManager();

	static SettlementManager* getInstance() { return instance; }
	static void setInstance(SettlementManager* inst) { instance = inst; }

	void addSettlement(Settlement* newSettlement);



	void deleteSettlement(Settlement* newSettlement);

	void removeSettlementsOutsideBounds(float minX, float minZ, float maxX, float maxZ);

	//Return every settlement within this circle radius center x and z.
	std::vector<Settlement*> getSettlementsInArea(float radius, float xPos, float zPos);

	std::vector<Settlement*> getSettlementsOnChunk(int gridX, int gridZ);

	float getNearestSettlementDist(glm::vec3 posIn);
	Settlement* getNearestSettlement(glm::vec3 posIn);

	Settlement* getSettlement(int settlementID);


public:

	//Minimum score a vertex can have to have a settlement focal point
	float minSettlementScore = 40.0f;

	NoiseGenerator* terrainNoiseGenerator;
private:

	//Store all currently generated settlements, including those not being renderer currently.
	static std::vector<Settlement*> settlements;
	int settlementIDIndex;

};

