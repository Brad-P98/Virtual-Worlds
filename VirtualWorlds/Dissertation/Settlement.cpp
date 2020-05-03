#include "Settlement.h"
#include "Building.h"

Settlement::Settlement(glm::vec3 focalPos, float score) : focalPointPos(focalPos), influenceScore(score)
{
	Building* building = new Building(focalPointPos, 1);
	generateSettlement();
}

Settlement::~Settlement()
{
}

void Settlement::generateSettlement()
{
}
