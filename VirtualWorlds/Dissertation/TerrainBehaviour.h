#pragma once
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
	}

private:

	glm::vec3 worldPos;		//Current position in the world
	glm::vec3 chunkPos;		//Current chunk that position is in

	Terrain* m_Terrain;

};

