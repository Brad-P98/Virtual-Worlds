#include "SettlementManager.h"

#include "ChunkSettings.h"

#include <assert.h>

SettlementManager* SettlementManager::instance;

std::vector<Settlement*> SettlementManager::settlements; 

SettlementManager::SettlementManager()
{
	assert(instance == nullptr);
	instance = this;
}

SettlementManager::~SettlementManager()
{
}

void SettlementManager::addSettlement(Settlement * newSettlement)
{
	settlements.push_back(newSettlement);

	newSettlement->settlementID = settlementIDIndex;
	//Build settlement
	newSettlement->constructSettlement();

	//Unique ID for next settlement.
	settlementIDIndex++;
}


void SettlementManager::deleteSettlement(Settlement * settlement)
{
	for (int i = 0; i < settlements.size(); i++) {
		if (settlements[i] == settlement) {
			settlements.erase(settlements.begin() + i);

			delete settlement;
			break;
		}
	}
}

void SettlementManager::removeSettlementsOutsideBounds(float minX, float minZ, float maxX, float maxZ)
{

	//Iterate through settlements
	//If settlement is outside bounds, delete
	for (int i = 0; i < settlements.size(); i++) {

		glm::vec3 currSettPos = settlements[i]->focalPointPos;
		if ((currSettPos.x > minX && currSettPos.z > minZ) && (currSettPos.x < maxX && currSettPos.z < maxZ)) {
			//Inside bounds
		}
		else {
			//Outside Bounds
			deleteSettlement(settlements[i]);
		}
	}
}

std::vector<Settlement*> SettlementManager::getSettlementsInArea(float radius, float xPos, float zPos)
{
	std::vector<Settlement*> settlementsFound;

	for (int i = 0; i < settlements.size(); i++) {

		float xDist = abs(settlements[i]->focalPointPos.x - xPos);
		float zDist = abs(settlements[i]->focalPointPos.z - zPos);

		float res = sqrt((xDist * xDist) + (zDist * zDist));
		if (res < radius && res > 0) {
			//settlement is within radius (and not itself), push to settlements found vector.
			settlementsFound.push_back(settlements[i]);
		}
	}

	return settlementsFound;
}

std::vector<Settlement*> SettlementManager::getSettlementsOnChunk(int gridX, int gridZ)
{
	std::vector<Settlement*> settlementsOnChunk;

	for (int i = 0; i < settlements.size(); i++) {

		if (settlements[i]->homeChunkCoords.x == gridX && settlements[i]->homeChunkCoords.z == gridZ) {
			settlementsOnChunk.push_back(settlements[i]);
		}
	}

	return settlementsOnChunk;
}

float SettlementManager::getNearestSettlementDist(glm::vec3 posIn)
{
	float nearestDist = -1.0f;
	//Retrieve the settlement that is closest to the position input.
	for (int i = 0; i < settlements.size(); i++) {

		float xDist = abs(settlements[i]->focalPointPos.x - posIn.x);
		float zDist = abs(settlements[i]->focalPointPos.z - posIn.z);

		//Dist between two points
		float res = sqrt((xDist * xDist) + (zDist * zDist));
		//If dist is shorter, replace
		if (res < nearestDist) nearestDist = res;
	}

	//Returns -1 if none found
	return nearestDist;
}

Settlement * SettlementManager::getNearestSettlement(glm::vec3 posIn)
{
	Settlement* nearestSett = nullptr;
	float nearestDist = 1000000.0f;
	//Retrieve the settlement that is closest to the position input.
	for (int i = 0; i < settlements.size(); i++) {

		float xDist = abs(settlements[i]->focalPointPos.x - posIn.x);
		float zDist = abs(settlements[i]->focalPointPos.z - posIn.z);

		//Dist between two points
		float res = sqrt((xDist * xDist) + (zDist * zDist));
		//If dist is shorter (and not self), replace
		if (res < nearestDist && res > 0) {
			nearestDist = res;
			nearestSett = settlements[i];
		}
	}
	return nearestSett;
}

Settlement * SettlementManager::getSettlement(int settlementID)
{
	for (int i = 0; i < settlements.size(); i++) {
		if (settlements[i]->settlementID == settlementID) {
			return settlements[i];
		}
	}
	//None found
	return nullptr;
}
