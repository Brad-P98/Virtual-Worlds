#pragma once
#include <vector>
#include "Settlement.h"

class SettlementManager;
static SettlementManager *settlementManager = nullptr;

class SettlementManager
{
public:
	SettlementManager();
	~SettlementManager();

	static SettlementManager* getInstance() { return settlementManager; }

	void addSettlement(Settlement* newSettlement);

	void removeSettlement(Settlement* newSettlement);

	//Return every settlement within this circle radius center x and z.
	std::vector<Settlement*> getSettlementsInArea(float radius, float xPos, float zPos);
private:

	//Store all currently generated settlements.
	static std::vector<Settlement*> activeSettlements;
};

