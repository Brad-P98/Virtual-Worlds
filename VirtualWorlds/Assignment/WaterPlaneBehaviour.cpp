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

	initializeChunks();
}

void WaterPlaneBehaviour::initializeChunks()
{
		//Generate rows of chunks on seperate threads.
		//When a row is done generating, finalize it.

		//Thread per chunk row. Not optimal efficiency but it'll do for now.
		threads.resize(renderDistance * 2 + 1);

		for (int i = 0; i < threads.size(); i++) {

			threads[i] = std::thread(&WaterPlane::generateXRow, m_waterPlane, chunkPos, i - renderDistance);
		}
		//Wait for thread to finish, then finalize the row.
		for (int i = 0; i < threads.size(); i++) {

			threads[i].join();

			m_waterPlane->finalizeXRow(i);
		}
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
			threadZ = std::thread(&WaterPlane::adjustZRow, m_waterPlane, false);
		}
		else if (chunkPos.z < prevChunkPos.z) {
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