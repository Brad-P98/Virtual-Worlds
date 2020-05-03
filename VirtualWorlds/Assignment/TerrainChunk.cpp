#include "TerrainChunk.h"
#include "Terrain.h"
#include "ChunkSettings.h"
#include "Rock.h"

#define chunkSize ChunkSettings::CHUNK_SIZE
#define vertexCount ChunkSettings::CHUNK_EDGE_VERTEX_COUNT
#define renderDistance ChunkSettings::CHUNK_RENDER_DISTANCE
#define seaLevel ChunkSettings::SEA_LEVEL

#pragma region TerrainChunks


TerrainChunk::TerrainChunk(int gridX, int gridZ, NoiseGenerator* noiseInterface, GLuint shader)
{
	m_gridX = gridX;
	m_gridZ = gridZ;
	m_noiseInterface = noiseInterface;
	m_shader = shader;

	x = gridX * chunkSize;
	z = gridZ * chunkSize;

	chunkMinXZ = glm::vec3(x, 0, z);
	//Default to pregenerated values.
	positions = Terrain::defaultVertexPositions;
	indices = Terrain::defaultVertexIndices;
	texCoords = Terrain::defaultTextureCoords;

	//Fill out positions and normals with unique values
	generateUniqueVertexPositions();
}

TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::generateVAO()
{
	VAOLoader* loader = VAOLoader::getInstance();

	std::vector<GLuint> texIDs;
	texIDs.push_back(TextureManager::getTextureID("Assets/grass_terrain.jpg"));
	texIDs.push_back(TextureManager::getTextureID("Assets/snow_terrain.jpg"));
	texIDs.push_back(TextureManager::getTextureID("Assets/gravel_terrain.jpg"));

	//standard
	init(loader->loadToVAO(positions, normals, indices, texCoords, terrainScores), texIDs, m_shader);
}

void TerrainChunk::updateScoresVBO()
{
	VAOLoader* loader = VAOLoader::getInstance();
	//Rewrite the scoreVBO of this object's VAO with the new score data in attribute index 5, with 1 component per vertex.
	loader->updateVBOInVAO(getVAOData()->getVaoID(), getVAOData()->scoreVBOID, 5, 1, terrainScores);
}


void TerrainChunk::generateUniqueVertexPositions()
{
	for (int i = 0, j = 1, k = 2; i < positions.size(); i += 3, j += 3, k += 3) {
		positions[i] += x;

		positions[k] += z;

		//y last
		positions[j] += m_noiseInterface->generateTotalNoise(positions[i], positions[k]);

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
		float h1 = m_noiseInterface->generateTotalNoise(nextXPos, positions[k]);
		float h2 = m_noiseInterface->generateTotalNoise(positions[i], nextZPos);
		float h3 = m_noiseInterface->generateTotalNoise(prevXPos, nextZPos);
		float h4 = m_noiseInterface->generateTotalNoise(prevXPos, positions[k]);
		float h5 = m_noiseInterface->generateTotalNoise(positions[i], prevZPos);
		float h6 = m_noiseInterface->generateTotalNoise(nextXPos, prevZPos);

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


		//VERTEX SCORE CALCULATION
		//float terrainVertexScore = 0.0f;
		//terrainVertexScore += calcGradientScore(positions[i], positions[k], vertexNormal);
		//terrainVertexScore += calcAltitudeScore(positions[i], positions[k]);

		//terrainScores.push_back(terrainVertexScore);
	}
}


void TerrainChunk::generateSurfaceDetail()
{

	for (int i = 1; i < positions.size(); i += 3) {
		//For every y position
		//If altitude is within these bounds
		if (positions[i] > seaLevel - 1 && positions[i] <= seaLevel + 5) {
			//Close to sea and/or just in water. fairly high chance of rock spawn
			tryGenRock(glm::vec3(positions[i-1], positions[i], positions[i+1]), 0.05f);
		}
		else if (positions[i] > seaLevel + 5 && positions[i] < seaLevel + 50) {
			//Middle height, low chance to generate rock
			tryGenRock(glm::vec3(positions[i - 1], positions[i], positions[i + 1]), 0.001f);

		}
		else if (positions[i] > seaLevel + 50) {
			//high height, slightly increased chance to generate rock
			tryGenRock(glm::vec3(positions[i - 1], positions[i], positions[i + 1]), 0.02f);

		}
		else {
			//Lets not generate any rocks below sea level
		}
	}

}

void TerrainChunk::tryGenRock(glm::vec3 pos, float chance)
{
	//normalize
	chance = glm::clamp(0.0f, 1.0f, chance) * 1000;

	//0-1000
	int randNum = rand() % 1001;
	if (chance == 1000) {
		//Gen rock always
		Rock* rock = new Rock(pos, 10000);
	}
	else if (randNum < chance) {
		//Gen rock
		Rock* rock = new Rock(pos, 10000);
	}
	else {
		//No rock generated
		return;
	}
}

//These calc methods are used to sample terrain vertices for 'scores'.
float TerrainChunk::calcGradientScore(float xPos, float zPos, glm::vec3 vertexNormal) 
{
	//if less than just above sea level, return no score
	if (m_noiseInterface->generateTotalNoise(xPos, zPos) <= seaLevel + 0.5f) return 0;
	
	//calc gradient based off vertex normal. If normal is close to up vector, dot is closer to 0.
	float grad = 1.0f - glm::dot(vertexNormal, glm::vec3(0, 1, 0));

	//The closer to flat, the higher the score, up to a cap.
	float gradScore = 1 / (grad * 40);
	if (gradScore > 20) return 20;
	return gradScore;
}


float TerrainChunk::calcAltitudeScore(float xPos, float zPos)
{
	float altitude = m_noiseInterface->generateTotalNoise(xPos, zPos);

	//if less than just above sea level, return no score
	if (altitude <= seaLevel + 0.2f) return 0;

	//The closer to sea level, the better the score, up to a cap
	float score = (1/(altitude - seaLevel)) * 300;
	if (score > 30) return 30;
	return score;
}
#pragma endregion