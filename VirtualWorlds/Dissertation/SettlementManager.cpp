#include "SettlementManager.h"

#include <assert.h>

SettlementManager* SettlementManager::instance;

std::vector<Settlement*> SettlementManager::activeSettlements; 

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
	activeSettlements.push_back(newSettlement);
}

void SettlementManager::removeSettlement(Settlement * settlement)
{
	for (int i = 0; i < activeSettlements.size(); i++) {
		if (activeSettlements[i] == settlement) {
			activeSettlements.erase(activeSettlements.begin() + i);

			delete settlement;

			break;
		}
	}
}

std::vector<Settlement*> SettlementManager::getSettlementsInArea(float radius, float xPos, float zPos)
{
	std::vector<Settlement*> settlementsFound;

	for (int i = 0; i < activeSettlements.size(); i++) {

		float xDist = abs(activeSettlements[i]->focalPointPos.x - xPos);
		float zDist = abs(activeSettlements[i]->focalPointPos.z - zPos);

		float res = sqrt((xDist * xDist) + (zDist * zDist));
		if (res < radius) {
			//settlement is within radius, push to settlements found vector.
			settlementsFound.push_back(activeSettlements[i]);
		}
	}

	return settlementsFound;
}
