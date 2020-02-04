#include "TerrainBehaviour.h"
#include <iostream>



TerrainBehaviour::TerrainBehaviour()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(worldPos.x / TerrainChunk::SIZE, 0, worldPos.z / TerrainChunk::SIZE);
}


TerrainBehaviour::~TerrainBehaviour()
{
}

void TerrainBehaviour::update()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();
	chunkPos = glm::vec3();

	std::cout << worldPos.x << " " << worldPos.y << " " << worldPos.z << std::endl;

	//Check if any terrain must be removed

	//Check if terrain must be added


}
