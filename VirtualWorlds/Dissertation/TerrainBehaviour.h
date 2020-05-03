#pragma once
#include <thread>

#include <Behaviour.h>
#include <Instance.h>

#include "Terrain.h"


class TerrainBehaviour : public Behaviour
{
public:
	TerrainBehaviour();
	~TerrainBehaviour();

	void update() override;

	void setActiveTerrain(Terrain* terrain) {
		m_Terrain = terrain;
		init();
	}

private:

	void init();

	void initializeChunks();

	//Generate settlements. Many of these will be eliminated later along in the process.
	void generateSettlements();

private:

	glm::vec3 worldPos;		//Current position in the world
	glm::vec3 chunkPos;		//Current chunk that position is in
	glm::vec3 prevChunkPos;	//When this is different to current, player is in new chunk

	Terrain* m_Terrain;

	std::vector<std::thread> threads;

	std::thread threadX;
	std::thread threadZ;

	bool terrainChanged = false;

private:
	std::vector<glm::vec3> eliminatePositions(std::vector<glm::vec3>, float);

};

