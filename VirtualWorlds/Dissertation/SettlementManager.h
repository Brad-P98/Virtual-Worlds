#pragma once
#include <vector>
#include <mutex>
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

	void removeSettlement(Settlement* newSettlement);

	//Return every settlement within this circle radius center x and z.
	std::vector<Settlement*> getSettlementsInArea(float radius, float xPos, float zPos);
private:

	//Store all currently generated settlements.
	static std::vector<Settlement*> activeSettlements;


public:

	//Minimum score a vertex can have to have a settlement focal point
	float minSettlementScore = 40.0f;
};

