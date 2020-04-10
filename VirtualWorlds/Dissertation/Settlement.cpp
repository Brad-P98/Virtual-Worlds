#include "Settlement.h"

Settlement::Settlement(glm::vec3 focalPos, float score) : focalPointPos(focalPos), influenceScore(score)
{

	generateSettlement();
}

Settlement::~Settlement()
{
}

void Settlement::generateSettlement()
{
}
