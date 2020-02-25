#include "Terrain.h"


const float TerrainChunk::SIZE = 100;
const int TerrainChunk::VERTEX_COUNT = 64;

std::vector<float> Terrain::defaultVertexPositions;
std::vector<GLuint> Terrain::defaultVertexIndices;

PerlinNoise* Terrain::noiseGenerator;
#pragma region Terrain
Terrain::Terrain()
{
	init();

	generateDefaultVertexPositions();
	generateDefaultVertexIndices();
}

Terrain::~Terrain()
{
}

void Terrain::init()
{
	noiseGenerator = new PerlinNoise();
}

void Terrain::generateDefaultVertexPositions()
{
	int vertexPointer = 0;
	for (int i = 0; i < TerrainChunk::VERTEX_COUNT; i++) {
		for (int j = 0; j < TerrainChunk::VERTEX_COUNT; j++) {
			//x
			defaultVertexPositions.push_back((float)j / ((float)TerrainChunk::VERTEX_COUNT - 1) * TerrainChunk::SIZE);
			//y
			defaultVertexPositions.push_back(0.0f);
			//z
			defaultVertexPositions.push_back((float)i / ((float)TerrainChunk::VERTEX_COUNT - 1) * TerrainChunk::SIZE);
		}
	}
}

void Terrain::generateDefaultVertexIndices()
{
	for (int i = 0; i < TerrainChunk::VERTEX_COUNT - 1; i++) {
		for (int j = 0; j < TerrainChunk::VERTEX_COUNT - 1; j++) {

			int topLeft = (i*TerrainChunk::VERTEX_COUNT) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * TerrainChunk::VERTEX_COUNT) + j;
			int bottomRight = bottomLeft + 1;

			defaultVertexIndices.push_back(topLeft);
			defaultVertexIndices.push_back(bottomLeft);
			defaultVertexIndices.push_back(topRight);
			defaultVertexIndices.push_back(topRight);
			defaultVertexIndices.push_back(bottomLeft);
			defaultVertexIndices.push_back(bottomRight);
		}
	}
}

void Terrain::generateInitChunks(glm::vec3 startChunkGridPos)
{
	activeTerrainChunks.resize(RENDER_DISTANCE_CHUNKS * 2 + 1, std::vector<TerrainChunk*>(RENDER_DISTANCE_CHUNKS * 2 + 1, nullptr));

	for (int i = 0; i < 2*RENDER_DISTANCE_CHUNKS + 1; i++) {
		for (int j = 0; j < 2 * RENDER_DISTANCE_CHUNKS + 1; j++) {

			TerrainChunk* newChunk = new TerrainChunk(startChunkGridPos.x + i-RENDER_DISTANCE_CHUNKS, startChunkGridPos.z + j - RENDER_DISTANCE_CHUNKS, shader);
			Instance::m_scene->addObject(newChunk);

			activeTerrainChunks[i][j] = newChunk;
		}
	}
}

//false = remove lowest x chunk, add higher x chunk
void Terrain::adjustXRow(bool direction)
{
	//new row's x position in grid
	int xPos = activeTerrainChunks[0][2 * RENDER_DISTANCE_CHUNKS + 1]->m_gridX;

	//new row'z starting z position in grid
	int zStart = activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS + 1][2 * RENDER_DISTANCE_CHUNKS + 1]->m_gridZ;

	//Erase the correct row of chunks from the vector
	if (!direction) {
		activeTerrainChunks.erase(activeTerrainChunks.begin());
	}
	else {
		activeTerrainChunks.erase(activeTerrainChunks.begin() + 2 * RENDER_DISTANCE_CHUNKS + 1);
	}

	//Create the new row of chunks
	std::vector<TerrainChunk*> tempChunkRow;
	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {
		

		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, shader);

		tempChunkRow.push_back(newChunk);


		if (!direction) {
			activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS + 1].push_back(newChunk);
		}
	}
	//Add the row of chunks to the correct end of the vector
	if (!direction) {
		activeTerrainChunks.push_back(tempChunkRow);
	}
	else {
		activeTerrainChunks.insert(activeTerrainChunks.begin(), tempChunkRow);
	}
}

void Terrain::adjustZRow(bool direction)
{
}



#pragma endregion

#pragma region TerrainChunks

TerrainChunk::TerrainChunk(int gridX, int gridZ, GLuint shader)
{
	m_gridX = gridX;
	m_gridZ = gridZ;

	x = gridX * SIZE;
	z = gridZ * SIZE;

	chunkMinXZ = glm::vec3(x, 0, z);

	//Default & standard
	positions = Terrain::defaultVertexPositions;
	indices = Terrain::defaultVertexIndices;

	//Fill out positions
	generateUniqueVertexPositions();

	VAOLoader loader;

	init(loader.loadToVAO(positions, indices), shader);

}

TerrainChunk::~TerrainChunk()
{
}
void TerrainChunk::generateUniqueVertexPositions()
{
	for (int i = 0, j = 1, k = 2; i < positions.size(); i+=3, j+=3, k+=3) {
		positions[i] += x;

		positions[k] += z;

		//y last
		positions[j] += Terrain::noiseGenerator->noise(positions[i], positions[k], 0.1f, NULL);

	}
}
#pragma endregion


