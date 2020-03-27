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

	void addSettlement(Settlement* newSettlement);

	void removeSettlement(Settlement* newSettlement);

	//Return every settlement on this chunk.
	void getSettlementsOnChunk(int chunkX, int chunkZ);

	//Return every settlement within this circle radius.
	void getSettlementsInArea(float radius);
private:

	//Store all currently generated settlements.
	static std::vector<Settlement*> activeSettlements;
};

