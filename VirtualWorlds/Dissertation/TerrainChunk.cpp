#include "TerrainChunk.h"
#include "Terrain.h"
#include "ChunkSettings.h"

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

	//Fill out positions and normals with unique values, as well as give every vertex a 'settlement score'
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
	//init(loader->loadToVAO(positions, normals, indices, texCoords, settlementInfluenceScores), texID, m_shader);
}

void TerrainChunk::updateScoresVBO()
{
	VAOLoader* loader = VAOLoader::getInstance();
	//Rewrite the scoreVBO of this object's VAO with the new score data in attribute index 5, with 1 component per vertex.
	loader->updateVBOInVAO(getVAOData()->getVaoID(), getVAOData()->scoreVBOID, 5, 1, terrainScores);
	//loader->updateVBOInVAO(getVAOData()->getVaoID(), getVAOData()->scoreVBOID, 5, 1, settlementInfluenceScores);
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
		float terrainVertexScore = 0.0f;
		terrainVertexScore += calcGradientScore(positions[i], positions[k], vertexNormal);
		terrainVertexScore += calcAltitudeScore(positions[i], positions[k]);

		terrainScores.push_back(terrainVertexScore);
		//Initialize at max score as no settlements are nearby. Will be updated later.
		settlementInfluenceScores.push_back(0.0f);
	}
}


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


float TerrainChunk::calcSettlementProxScore(glm::vec3 position)
{

	//Get all settlements within a suitable radius of this vertex.
	//Evaluate the 'effect' each settlement has on this vertex, based on distance and it's influence score.
	SettlementManager* sMgr = SettlementManager::getInstance();
	std::vector<Settlement*> settlements = sMgr->getSettlementsInArea(150, position.x, position.z);

	float totalScore = 0.0f;
	for (int i = 0; i < settlements.size(); i++) {

		//iterate through each settlement
		float distance =  5 *glm::distance(settlements[i]->focalPointPos, position);
		float score = ( settlements[i]->influenceScore / distance) * 20 ;
		totalScore += score;
	}

	return totalScore;

}


//Stride will change the number of vertices skipped. Improves speed of generation when increased.
//Generate settlements based on terrain only
std::vector<glm::vec3> TerrainChunk::generateSettlementPositions(int stride)
{
	std::vector<glm::vec3> potentialPositions;

	//Iterate through vertices
	for (int i = 0; i < terrainScores.size(); i+= stride) {
		if (terrainScores[i] > 49) {

			potentialPositions.push_back(glm::vec3(positions[i * 3], positions[(i * 3) + 1], positions[(i * 3) + 2]));
		}
	}

	return potentialPositions;
}

//Generate settlements based heavily on how far away from other settlements they are.
std::vector<glm::vec3> TerrainChunk::generateSettlementPositionsProxBased(int stride)
{
	std::vector<glm::vec3> potentialPositions;

	//Iterate through vertices
	for (int i = 0; i < terrainScores.size(); i += stride) {
		glm::vec3 currPos = glm::vec3(positions[i * 3], positions[(i * 3) + 1], positions[(i * 3) + 2]);
		//If terrain score is not horrific, and no settlements nearby, make candidate.
		float nearestSettDist = SettlementManager::getInstance()->getNearestSettlementDist(currPos);
		if (terrainScores[i] > 25 && (nearestSettDist > 300 || nearestSettDist == -1.0f)) {

			potentialPositions.push_back(currPos);
		}
	}

	return potentialPositions;
}


void TerrainChunk::resampleVertexScores(bool settlementsOnly, int vertexStride)
{

	//Empty score vectors
	if (settlementsOnly) {
		settlementInfluenceScores.clear();
	}
	else {
		terrainScores.clear();
		settlementInfluenceScores.clear();
	}


	//Iterate through vertices. stride will skip vertices, and 
	for (int i = 0; i < positions.size()/3; i+=vertexStride) {

		//Only recalc settlements
		if (settlementsOnly) {
			float settlementVertexScore = 0.0f;
			settlementVertexScore += calcSettlementProxScore(glm::vec3(positions[i * 3], positions[(i * 3) + 1], positions[(i * 3) + 2]));
			settlementInfluenceScores.push_back(settlementVertexScore);

		}
		//Recalc all
		else {
			//TOTAL VERTEX SCORE CALCULATION
			float terrainVertexScore = 0.0f;
			float settlementVertexScore = 0.0f;

			terrainVertexScore += calcGradientScore(positions[i * 3], positions[i * 3], glm::vec3(normals[i*3], normals[(i*3) + 1], normals[(i * 3) + 2]));
			terrainVertexScore += calcAltitudeScore(positions[i], positions[(i * 3) + 2]);

			settlementVertexScore += calcSettlementProxScore(glm::vec3(positions[i * 3], positions[(i * 3) + 1], positions[(i * 3) + 2]));

			terrainScores.push_back(terrainVertexScore);
			settlementInfluenceScores.push_back(settlementVertexScore);
		}

	}
	updateScoresVBO();
}

#pragma endregion