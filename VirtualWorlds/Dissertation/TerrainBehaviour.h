#pragma once
#include <Behaviour.h>
#include <Instance.h>


class TerrainBehaviour : public Behaviour
{
public:
	TerrainBehaviour();
	~TerrainBehaviour();

	void update() override;

private:

	glm::vec3 worldPos;		//Current position in the world
	glm::vec3 chunkPos;		//Current chunk that position is in


};

