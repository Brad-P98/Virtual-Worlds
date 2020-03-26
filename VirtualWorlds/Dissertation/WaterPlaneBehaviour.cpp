#include "WaterPlaneBehaviour.h"
#include "ChunkSettings.h"


#define renderDistance ChunkSettings::CHUNK_RENDER_DISTANCE
#define chunkSize ChunkSettings::CHUNK_SIZE


WaterPlaneBehaviour::WaterPlaneBehaviour()
{

	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / chunkSize + 0.499f) - 1, 0, round(worldPos.z / chunkSize + 0.499f) - 1);
}

void WaterPlaneBehaviour::init()
{
	//generate chunks, centered around chunk pos
	m_waterPlane->generateInitChunks(chunkPos);
}

WaterPlaneBehaviour::~WaterPlaneBehaviour()
{

}

void WaterPlaneBehaviour::update()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / chunkSize + 0.499f) - 1, 0, round(worldPos.z / chunkSize + 0.499f) - 1);

	if (chunkPos != prevChunkPos && (m_waterPlane->idleX && m_waterPlane->idleZ)) {

		//At a new chunk
		if (chunkPos.x > prevChunkPos.x) {
			//moved +x, so delete row of chunks with lowest x, and add row after highest x
			threadX = std::thread(&WaterPlane::adjustXRow, m_waterPlane, false);
		}
		else if (chunkPos.x < prevChunkPos.x) {
			//moved -x, so delete row of chunks with highest x, and add row before lowest x
			threadX = std::thread(&WaterPlane::adjustXRow, m_waterPlane, true);
		}

		if (chunkPos.z > prevChunkPos.z) {
			//m_Terrain->adjustZRow(false);
			threadZ = std::thread(&WaterPlane::adjustZRow, m_waterPlane, false);
		}
		else if (chunkPos.z < prevChunkPos.z) {
			//m_Terrain->adjustZRow(true);
			threadZ = std::thread(&WaterPlane::adjustZRow, m_waterPlane, true);
		}
	}


	prevChunkPos = chunkPos;


	//Check if generation is finished in order to add the chunks to the scene
	if (m_waterPlane->doneGeneratingX) {
		m_waterPlane->doneGeneratingX = false;

		threadX.join();

		m_waterPlane->finalizeXGeneration();
	}
	if (m_waterPlane->doneGeneratingZ) {
		m_waterPlane->doneGeneratingZ = false;

		threadZ.join();
		m_waterPlane->finalizeZGeneration();
	}

}