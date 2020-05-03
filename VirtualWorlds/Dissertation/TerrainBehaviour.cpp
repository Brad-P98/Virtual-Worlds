#include "TerrainBehaviour.h"
#include "ChunkSettings.h"

#define renderDistance ChunkSettings::CHUNK_RENDER_DISTANCE
#define chunkSize ChunkSettings::CHUNK_SIZE


TerrainBehaviour::TerrainBehaviour()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / chunkSize + 0.499f) - 1, 0, round(worldPos.z / chunkSize + 0.499f) - 1);

}

TerrainBehaviour::~TerrainBehaviour()
{
}

//Called after the active terrain has been set
void TerrainBehaviour::init()
{
	initializeChunks();
	generateSettlements();
	//m_Terrain->TryGenerateNewSettlements();
}

void TerrainBehaviour::initializeChunks()
{
	//Generate rows of chunks on seperate threads.
	//When a row is done generating, finalize it.

	//Thread per chunk row. Not optimal efficiency but it'll do for now.
	threads.resize(renderDistance * 2 + 1);

	for (int i = 0; i < threads.size(); i++) {

		threads[i] = std::thread(&Terrain::generateXRow, m_Terrain, chunkPos, i - renderDistance);
	}
	//Wait for thread to finish, then finalize the row.
	for (int i = 0; i < threads.size(); i++) {

		threads[i].join();

		m_Terrain->finalizeXRow(i);
	}

}


void TerrainBehaviour::update()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	chunkPos = glm::vec3(round(worldPos.x / chunkSize + 0.499f) - 1, 0, round(worldPos.z / chunkSize + 0.499f) - 1);

	//Only try to generate settlements if something changed e.g. moved to a new chunk.
	if(terrainChanged) generateSettlements();


	if (chunkPos != prevChunkPos && (m_Terrain->idleX && m_Terrain->idleZ)) {

		//moved to a new chunk.
		//delete the row of chunks that is now outside the render distance,
		//and add a new row within the render distance
		if (chunkPos.x > prevChunkPos.x) {
			//moved +x, so delete row of chunks with lowest x, and add row after highest x
			threadX = std::thread(&Terrain::adjustXRow, m_Terrain, false);
		}
		else if (chunkPos.x < prevChunkPos.x) {
			//moved -x, so delete row of chunks with highest x, and add row before lowest x
			threadX = std::thread(&Terrain::adjustXRow, m_Terrain, true);

		}

		if (chunkPos.z > prevChunkPos.z) {
			//m_Terrain->adjustZRow(false);
			threadZ = std::thread(&Terrain::adjustZRow, m_Terrain, false);
		}
		else if (chunkPos.z < prevChunkPos.z) {
			//m_Terrain->adjustZRow(true);
			threadZ = std::thread(&Terrain::adjustZRow, m_Terrain, true);
		}
		terrainChanged = true;
	}

	prevChunkPos = chunkPos;

	//Check if generation is finished in order to add the chunks to the scene
	if (m_Terrain->doneGeneratingX) {
		m_Terrain->doneGeneratingX = false;

		threadX.join();

		m_Terrain->finalizeXGeneration();
		terrainChanged = false;
	}
	if (m_Terrain->doneGeneratingZ) {
		m_Terrain->doneGeneratingZ = false;

		threadZ.join();
		m_Terrain->finalizeZGeneration();
		terrainChanged = false;
	}
}

#pragma region SETTLEMENT GENERATION
void TerrainBehaviour::generateSettlements()
{

	std::vector<std::vector<TerrainChunk*>> activeChunks = m_Terrain->getActiveTerrainChunks();

	//Store those that are certain to be there.
	std::vector<glm::vec3> finalSettlementPositions;

	//Temp array
	std::vector<glm::vec3> potentialSettlementPositions;
	//FIRST PASS ----- Generate  temporary settlement positions based on terrain.
	//Settlements will be generated near coastal areas, with a tendency towards flatter land.
	for (int i = 0; i < activeChunks.size(); i++) {
		for (int j = 0; j < activeChunks[i].size(); j++) {

			if (!activeChunks[i][j]->settlementsGenerated) {

				//Generate settlement positions on this chunk based on terrain
				std::vector<glm::vec3> positions = activeChunks[i][j]->generateSettlementPositions(5);
				//Fill vector with result
				for (int k = 0; k < positions.size(); k++) {
					potentialSettlementPositions.push_back(positions[k]);
				}
			}
		}
	}
	potentialSettlementPositions = eliminatePositions(potentialSettlementPositions, 100);
	//Push these into the final vector.
	for (int i = 0; i < potentialSettlementPositions.size(); i++) {
		finalSettlementPositions.push_back(potentialSettlementPositions[i]);
	}


	//Clear temp array
	potentialSettlementPositions.clear();
	//SECOND PASS ----- Generate based mainly on distance from other settlements. High stride as less accuracy needed.
	for (int i = 0; i < activeChunks.size(); i++) {
		for (int j = 0; j < activeChunks[i].size(); j++) {
			if (!activeChunks[i][j]->settlementsGenerated) {
				//Generate settlement positions on this chunk based on other settlements
				std::vector<glm::vec3> positions = activeChunks[i][j]->generateSettlementPositionsProxBased(25);
				//Fill vector with result
				for (int k = 0; k < positions.size(); k++) {
					potentialSettlementPositions.push_back(positions[k]);
				}
			}
		}
	}
	potentialSettlementPositions = eliminatePositions(potentialSettlementPositions, 200);
	//Push these into the final vector.
	for (int i = 0; i < potentialSettlementPositions.size(); i++) {
		finalSettlementPositions.push_back(potentialSettlementPositions[i]);
	}



	//Generate settlements out of those remaining.
	for (int i = 0; i < potentialSettlementPositions.size(); i++) {
		Settlement* newSett = new Settlement(potentialSettlementPositions[i], 150);
		SettlementManager::getInstance()->addSettlement(newSett);
	}

}

std::vector<glm::vec3> TerrainBehaviour::eliminatePositions(std::vector<glm::vec3> positions, float radius)
{
	std::vector<glm::vec3> finalPositions;

	//While the array is not empty, look for groupings.
	while (!positions.empty()) {

		std::vector<glm::vec3> nearCurrPos;		//Push nearby positions into here
		glm::vec3 currPos = positions[0];		//First element
		nearCurrPos.push_back(currPos);			//Need to take this one into account too
		positions.erase(positions.begin());		//Erase this element from overall vector
		int index = 0;
		//Iterate through every position. The vector size changes, so use .end()
		while (index < positions.size()) {

			float xDist = abs(positions[index].x - currPos.x);
			float zDist = abs(positions[index].z - currPos.z);
			float res = sqrt((xDist * xDist) + (zDist * zDist));
			//If position within bounds
			if (res < radius) {
				//Add to nearby vec
				nearCurrPos.push_back(positions[index]);
				//remove from positions
				positions.erase(positions.begin() + index);
			}
			else {
				index++;
			}

		}
		//We now have every position near this vertex, and they have been removed from the original vector.

		//Calculate average x and z
		glm::vec3 averagePosition(0,0,0);
		float xTotal = 0.0f;
		float zTotal = 0.0f;
		for (int i = 0; i < nearCurrPos.size(); i++) {
			xTotal += nearCurrPos[i].x;
			zTotal += nearCurrPos[i].z;
		}
		averagePosition.x = xTotal / nearCurrPos.size();
		averagePosition.z = zTotal / nearCurrPos.size();

		//Get y value here
		averagePosition.y = m_Terrain->getYAtPosition(averagePosition.x, averagePosition.z);
		

		//Push final position to final vector
		finalPositions.push_back(averagePosition);
		//Repeat for next.
	}


	return finalPositions;
}



#pragma endregion

