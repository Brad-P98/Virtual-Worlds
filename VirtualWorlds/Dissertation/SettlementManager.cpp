#include "SettlementManager.h"

#include <assert.h>

std::vector<Settlement*> SettlementManager::activeSettlements;

SettlementManager::SettlementManager()
{
	assert(settlementManager == nullptr);
	settlementManager = this;
}

SettlementManager::~SettlementManager()
{
}