#include "Terrain.h"


const float TerrainChunk::SIZE = 100;
const int TerrainChunk::VERTEX_COUNT = 128;

std::vector<float> Terrain::defaultVertexPositions;
std::vector<GLuint> Terrain::defaultVertexIndices;

PerlinNoise* Terrain::noiseGenerator;
#pragma region Terrain
Terrain::Terrain()
{
	init();

	GenerateDefaultVertexPositions();
	GenerateDefaultVertexIndices();
}

Terrain::~Terrain()
{
}

void Terrain::init()
{
	noiseGenerator = new PerlinNoise();
}

void Terrain::GenerateDefaultVertexPositions()
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

void Terrain::GenerateDefaultVertexIndices()
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

void Terrain::GenerateInitChunks(glm::vec3 startChunkGridPos)
{
	//TerrainChunk* newChunk = new TerrainChunk(startChunkGridPos.x, startChunkGridPos.z, shader);
	//Instance::m_scene->addObject(newChunk);

	activeTerrainChunks.resize(RENDER_DISTANCE_CHUNKS * 2 + 1, std::vector<TerrainChunk*>(RENDER_DISTANCE_CHUNKS * 2 + 1, nullptr));

	for (int i = 0; i < 2*RENDER_DISTANCE_CHUNKS + 1; i++) {
		for (int j = 0; j < 2 * RENDER_DISTANCE_CHUNKS + 1; j++) {

			TerrainChunk* newChunk = new TerrainChunk(startChunkGridPos.x + i-RENDER_DISTANCE_CHUNKS, startChunkGridPos.z + j - RENDER_DISTANCE_CHUNKS, shader);
			Instance::m_scene->addObject(newChunk);

			activeTerrainChunks[i][j] = newChunk;
		}
	}

}



#pragma endregion

#pragma region TerrainChunks

TerrainChunk::TerrainChunk(int gridX, int gridZ, GLuint shader)
{
	x = gridX * SIZE;
	z = gridZ * SIZE;

	chunkCentre = glm::vec3(x - (SIZE / 2), 0, z - (SIZE / 2));

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


