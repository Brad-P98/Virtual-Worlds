#include "TerrainChunk.h"
#include "Terrain.h"
#include "ChunkSettings.h"

#define chunkSize ChunkSettings::CHUNK_SIZE
#define vertexCount ChunkSettings::CHUNK_EDGE_VERTEX_COUNT
#define renderDistance ChunkSettings::CHUNK_RENDER_DISTANCE
#define seaLevel ChunkSettings::SEA_LEVEL

#pragma region TerrainChunks


TerrainChunk::TerrainChunk(int gridX, int gridZ, GLuint shader)
{
	m_gridX = gridX;
	m_gridZ = gridZ;

	m_shader = shader;

	x = gridX * chunkSize;
	z = gridZ * chunkSize;

	chunkMinXZ = glm::vec3(x, 0, z);

	//Default to pregenerated values.
	positions = Terrain::defaultVertexPositions;
	indices = Terrain::defaultVertexIndices;
	texCoords = Terrain::defaultTextureCoords;

	//Fill out positions and normals
	generateUniqueVertexPositions();

}

TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::generateVAO()
{
	VAOLoader* loader = VAOLoader::getInstance();

	GLuint texID = TextureManager::getTextureID("Assets/grass_terrain.jpg");
	init(loader->loadToVAO(positions, normals, indices, texCoords, scores), texID, m_shader);
}

void TerrainChunk::generateUniqueVertexPositions()
{
	for (int i = 0, j = 1, k = 2; i < positions.size(); i += 3, j += 3, k += 3) {
		positions[i] += x;

		positions[k] += z;

		//y last
		positions[j] += TerrainNoise::generateTotalNoise(positions[i], positions[k]);

		//VERTEX NORMAL CALCULATION
		//Calculate normal for this point
		int h0 = positions[j];	//Height at this point

		glm::vec3 p0 = glm::vec3(positions[i], h0, positions[k]);

		//Info about surrounding points
		float vertexSeperationDist = chunkSize / vertexCount;
		float nextXPos = positions[i] + vertexSeperationDist;
		float nextZPos = positions[k] + vertexSeperationDist;
		float prevXPos = positions[i] - vertexSeperationDist;
		float prevZPos = positions[k] - vertexSeperationDist;

		//Height of surrounding points, clockwise
		float h1 = TerrainNoise::generateTotalNoise(nextXPos, positions[k]);
		float h2 = TerrainNoise::generateTotalNoise(positions[i], nextZPos);
		float h3 = TerrainNoise::generateTotalNoise(prevXPos, nextZPos);
		float h4 = TerrainNoise::generateTotalNoise(prevXPos, positions[k]);
		float h5 = TerrainNoise::generateTotalNoise(positions[i], prevZPos);
		float h6 = TerrainNoise::generateTotalNoise(nextXPos, prevZPos);

		//vector for each point
		glm::vec3 p1 = glm::vec3(nextXPos, h1, positions[k]);
		glm::vec3 p2 = glm::vec3(positions[i], h2, nextZPos);
		glm::vec3 p3 = glm::vec3(prevXPos, h3, nextZPos);
		glm::vec3 p4 = glm::vec3(prevXPos, h4, positions[k]);
		glm::vec3 p5 = glm::vec3(positions[i], h5, prevZPos);
		glm::vec3 p6 = glm::vec3(nextXPos, h6, prevZPos);


		//calculate surface normal direction of the 6 surrounding triangles
		glm::vec3 n1 = glm::normalize(glm::cross(p1 - p0, p2 - p0));
		glm::vec3 n2 = glm::normalize(glm::cross(p2 - p0, p3 - p0));
		glm::vec3 n3 = glm::normalize(glm::cross(p3 - p0, p4 - p0));
		glm::vec3 n4 = glm::normalize(glm::cross(p4 - p0, p5 - p0));
		glm::vec3 n5 = glm::normalize(glm::cross(p5 - p0, p6 - p0));
		glm::vec3 n6 = glm::normalize(glm::cross(p6 - p0, p1 - p0));

		//calculate the average direction of the surface normals
		glm::vec3 vertexNormal = -glm::normalize(n1 + n2 + n3 + n4 + n5 + n6);

		//push the normal data into the vector
		normals.push_back(vertexNormal.x);
		normals.push_back(vertexNormal.y);
		normals.push_back(vertexNormal.z);


		//Calculate score of vertex
		float vertexScore = 0.0f;
		vertexScore += calcGradientScore(positions[i], positions[k], vertexNormal);
		vertexScore += calcAltitudeScore(positions[i], positions[k]);
		vertexScore += calcSettlementProxScore(positions[i], positions[k]);

		scores.push_back(vertexScore);
	}
}

float TerrainChunk::calcGradientScore(float xPos, float zPos, glm::vec3 vertexNormal) 
{
	//if less than just above sea level, return no score
	if (TerrainNoise::generateTotalNoise(xPos, zPos) <= seaLevel + 0.5f) return 0;
	
	//calc gradient based off vertex normal
	float grad = 1.0f - glm::dot(vertexNormal, glm::vec3(0, 1, 0));

	//The closer to flat, the higher the score, up to a cap.
	grad = 1 / (grad * 60);
	if (grad > 10) return 10;
	return grad;
}


float TerrainChunk::calcAltitudeScore(float xPos, float zPos)
{
	float altitude = TerrainNoise::generateTotalNoise(xPos, zPos);

	//if less than just above sea level, return no score
	if (altitude <= seaLevel + 0.2f) return 0;

	//The closer to sea level, the better the score, up to a cap
	float score = (1/(altitude - seaLevel)) * 15;
	if (score > 10) return 10;
	return score;
}
float TerrainChunk::calcSettlementProxScore(float xPos, float zPos)
{
	return 0.0f;
}
#pragma endregion