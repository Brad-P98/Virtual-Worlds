#include "TerrainBehaviour.h"
#include <iostream>



TerrainBehaviour::TerrainBehaviour()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / TerrainChunk::SIZE + 0.499f) - 1, 0, round(worldPos.z / TerrainChunk::SIZE + 0.499f) - 1);


}

//Called after the active terrain has been set
void TerrainBehaviour::init()
{
	m_Terrain->generateInitChunks(chunkPos);
}

TerrainBehaviour::~TerrainBehaviour()
{
}

void TerrainBehaviour::update()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / TerrainChunk::SIZE + 0.499f) - 1, 0, round(worldPos.z / TerrainChunk::SIZE + 0.499f) - 1);

	if (chunkPos != prevChunkPos) {
		std::cout << "moved";

		//moved to a new chunk.
		//delete the row of chunks that is now outside the render distance,
		//and add a new row within the render distance
		if (chunkPos.x > prevChunkPos.x) {
			//moved +x, so delete row of chunks with lowest x, and add row after highest x
			m_Terrain->adjustXRow(false);
		}
		if (chunkPos.x < prevChunkPos.x) {
			//moved -x, so delete row of chunks with highest x, and add row before lowest x
			m_Terrain->adjustXRow(true);

		}

		if (chunkPos.z > prevChunkPos.z) {
			m_Terrain->adjustZRow(false);
		}
		if (chunkPos.z < prevChunkPos.z) {
			m_Terrain->adjustZRow(true);

		}
	}


	prevChunkPos = chunkPos;
}


