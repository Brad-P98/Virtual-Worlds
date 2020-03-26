#pragma once
#include <thread>

#include <Behaviour.h>
#include <Instance.h>

#include "WaterPlane.h"

class WaterPlaneBehaviour : public Behaviour
{
public:
	WaterPlaneBehaviour();
	~WaterPlaneBehaviour();

	void update() override;

	void setActiveWaterPlane(WaterPlane* water)
	{
		m_waterPlane = water;
		init();
	}

private:

	void init();

	void initializeChunks();
private:

	glm::vec3 worldPos;
	glm::vec3 chunkPos;
	glm::vec3 prevChunkPos;

	WaterPlane* m_waterPlane;

	std::vector<std::thread> threads;

	std::thread threadX;
	std::thread threadZ;
};

