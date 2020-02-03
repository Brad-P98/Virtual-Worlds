#include "Terrain.h"


#pragma region TerrainChunks

TerrainChunk::TerrainChunk(int gridX, int gridZ, GLuint shader)
{

	x = gridX * SIZE;
	z = gridZ * SIZE;

	generateVertices();
	generateIndices();

	VAOLoader loader;
	init(loader.loadToVAO(positions, indices), shader);
}


TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::generateVertices()
{
	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++) {
		for (int j = 0; j < VERTEX_COUNT; j++) {

			positions.push_back( -(float)j / ((float)VERTEX_COUNT - 1) * SIZE);
			positions.push_back(0.0f);
			positions.push_back(-(float)i / ((float)VERTEX_COUNT - 1) * SIZE);
		}
	}
}

void TerrainChunk::generateIndices() 
{

	for (int i = 0; i < VERTEX_COUNT - 1; i++) {
		for (int j = 0; j < VERTEX_COUNT - 1; j++) {

			int topLeft = (i*VERTEX_COUNT) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * VERTEX_COUNT) + j;
			int bottomRight = bottomLeft + 1;

			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
}
#pragma endregion