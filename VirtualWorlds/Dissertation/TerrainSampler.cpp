#include "TerrainSampler.h"

#define vertexCount ChunkSettings::CHUNK_EDGE_VERTEX_COUNT
#define chunkSize ChunkSettings::CHUNK_SIZE
#define seaLevel ChunkSettings::SEA_LEVEL

std::vector<int> TerrainSampler::sampleChunk(TerrainChunk* chunk)
{

	std::vector<int> scores;
	scores.resize(vertexCount * vertexCount);

	int xPos, zPos;
	//For every vertex in the chunk
	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {
			
			int vertexScore = 0;

			//Calculate world coords of the current vertex
			xPos = chunk->chunkMinXZ.x + (float)j / ((float)vertexCount - 1) * chunkSize;
			zPos = chunk->chunkMinXZ.z + (float)i / ((float)vertexCount - 1) * chunkSize;

			vertexScore += calcGradientScore(xPos, zPos);
			vertexScore += calcAltitudeScore(xPos, zPos);
			vertexScore += calcSettlementProxScore(xPos, zPos);

			scores.push_back(vertexScore);

		}
	}



	return std::vector<int>();
}

int TerrainSampler::calcGradientScore(float xPos, float zPos)
{
	return 0;
}

int TerrainSampler::calcAltitudeScore(float xPos, float zPos)
{
	return 0;
}

int TerrainSampler::calcSettlementProxScore(float xPos, float zPos)
{
	return 0;
}
