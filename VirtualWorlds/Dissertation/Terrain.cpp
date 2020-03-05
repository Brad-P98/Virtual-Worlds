#include "Terrain.h"


const float TerrainChunk::SIZE = 100;
const int TerrainChunk::VERTEX_COUNT = 32;

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

void Terrain::generateDefaultVertexNormals()
{
	for (int i = 0; i < TerrainChunk::VERTEX_COUNT; i++) {
		for (int j = 0; j < TerrainChunk::VERTEX_COUNT; j++) {
			//x
			defaultVertexPositions.push_back(0.0f);
			//y
			defaultVertexPositions.push_back(1.0f);
			//z
			defaultVertexPositions.push_back(0.0f);
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

			newChunk->generateVAO();
			Instance::m_scene->addObject(newChunk);

			activeTerrainChunks[i][j] = newChunk;
		}
	}
}

//false = remove lowest x chunk, add higher x chunk
void Terrain::adjustXRow(bool direction)
{

	if (!idleX) return;
	idleX = false;
	//new row's x position in grid
	int xPos;
	if (!direction) {

		xPos = activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS][0]->m_gridX + 1;
	}
	else {

		xPos = activeTerrainChunks[0][0]->m_gridX - 1;
	}


	//new row's starting z position in grid
	int zStart = activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS][0]->m_gridZ;

	//Erase the correct row of chunks from the vector
	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeTerrainChunks[0].size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[0][i]);
			chunksToRemoveX.push_back(activeTerrainChunks[0][i]);
		}
		//remove from vector
		activeTerrainChunks.erase(activeTerrainChunks.begin());
	}
	else {
		for (int i = 0; i < activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS].size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS][i]);
			chunksToRemoveX.push_back(activeTerrainChunks[2 * RENDER_DISTANCE_CHUNKS][i]);
		}
		activeTerrainChunks.erase(activeTerrainChunks.begin() + 2 * RENDER_DISTANCE_CHUNKS);
	}

	//Create the new row of chunks
	std::vector<TerrainChunk*> tempChunkRow;
	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {
		

		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, shader);

		tempChunkRow.push_back(newChunk);
		//Instance::m_scene->addObject(newChunk);
		chunksToAddX.push_back(newChunk);
	}
	//Add the row of chunks to the correct end of the vector
	if (!direction) {
		activeTerrainChunks.push_back(tempChunkRow);
	}
	else {
		activeTerrainChunks.insert(activeTerrainChunks.begin(), tempChunkRow);
	}
	idleX = true;
	doneGeneratingX = true;
}

void Terrain::adjustZRow(bool direction)
{
	if (!idleZ) return;
	idleZ = false;
	int zPos;
	if (!direction) {
		zPos = activeTerrainChunks[0][2 * RENDER_DISTANCE_CHUNKS]->m_gridZ + 1;
	}
	else {
		zPos = activeTerrainChunks[0][0]->m_gridZ - 1;

	}

	int xStart = activeTerrainChunks[0][0]->m_gridX;

	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeTerrainChunks.size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[i][0]);
			chunksToRemoveZ.push_back(activeTerrainChunks[i][0]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin());
		}
	}
	else {
		for (int i = 0; i < activeTerrainChunks.size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[i][2*RENDER_DISTANCE_CHUNKS]);
			chunksToRemoveZ.push_back(activeTerrainChunks[i][2 * RENDER_DISTANCE_CHUNKS]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin() + 2*RENDER_DISTANCE_CHUNKS);
		}
	}
	activeTerrainChunks;

	//Create all new chunks and push into vector
	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {
	
		TerrainChunk* newChunk = new TerrainChunk(xStart + i, zPos, shader);
		//Instance::m_scene->addObject(newChunk);

		chunksToAddZ.push_back(newChunk);

		if (!direction) {
			activeTerrainChunks[i].push_back(newChunk);
		}
		else {
			activeTerrainChunks[i].insert(activeTerrainChunks[i].begin(), newChunk);
		}

	}
	idleZ = true;
	doneGeneratingZ = true;
}

void Terrain::finalizeXGeneration()
{

	for (int i = 0; i < chunksToAddX.size(); i++) {

		chunksToAddX[i]->generateVAO();
		Instance::m_scene->addObject(chunksToAddX[i]);
	}

	for (int i = 0; i < chunksToRemoveX.size(); i++) {

		Instance::m_scene->removeObject(chunksToRemoveX[i]);
	}
	
	chunksToAddX.clear();
	chunksToRemoveX.clear();
}

void Terrain::finalizeZGeneration()
{

	for (int i = 0; i < chunksToAddZ.size(); i++) {

		chunksToAddZ[i]->generateVAO();
		Instance::m_scene->addObject(chunksToAddZ[i]);
	}

	for (int i = 0; i < chunksToRemoveZ.size(); i++) {

		Instance::m_scene->removeObject(chunksToRemoveZ[i]);

	}
	chunksToAddZ.clear();
	chunksToRemoveZ.clear();
}



#pragma endregion


#pragma region TerrainChunks

TerrainChunk::TerrainChunk(int gridX, int gridZ, GLuint shader)
{
	m_gridX = gridX;
	m_gridZ = gridZ;

	m_shader = shader;

	x = gridX * SIZE;
	z = gridZ * SIZE;

	chunkMinXZ = glm::vec3(x, 0, z);

	//Default & standard
	positions = Terrain::defaultVertexPositions;
	indices = Terrain::defaultVertexIndices;

	//Fill out positions and normals
	generateUniqueVertexPositions();

}

TerrainChunk::~TerrainChunk()
{
}

void TerrainChunk::generateVAO()
{
	VAOLoader* loader = VAOLoader::getInstance();

	init(loader->loadToVAO(positions, normals, indices), m_shader);
}

void TerrainChunk::generateUniqueVertexPositions()
{
	float factor = 0.002f;
	float factor2 = 0.02f;
	for (int i = 0, j = 1, k = 2; i < positions.size(); i+=3, j+=3, k+=3) {
		positions[i] += x;

		positions[k] += z;

		//y last
		positions[j] += generateTotalNoise(positions[i], positions[k]);

		//VERTEX NORMAL CALCULATION
		//Calculate normal for this point
		int h0 = positions[j];	//Height at this point

		glm::vec3 p0 = glm::vec3(positions[i], h0, positions[k]);

		//Info about surrounding points
		float vertexSeperationDist = TerrainChunk::SIZE / TerrainChunk::VERTEX_COUNT;
		float nextXPos = positions[i] + vertexSeperationDist;
		float nextZPos = positions[k] + vertexSeperationDist;
		float prevXPos = positions[i] - vertexSeperationDist;
		float prevZPos = positions[k] - vertexSeperationDist;

		//Height of surrounding points, clockwise
		float h1 = generateTotalNoise(nextXPos, positions[k]);
		float h2 = generateTotalNoise(positions[i], nextZPos);
		float h3 = generateTotalNoise(prevXPos, nextZPos);
		float h4 = generateTotalNoise(prevXPos, positions[k]);
		float h5 = generateTotalNoise(positions[i], prevZPos);
		float h6 = generateTotalNoise(nextXPos, prevZPos);

		//vector for each point
		glm::vec3 p1 = glm::vec3(nextXPos,		h1, positions[k]);
		glm::vec3 p2 = glm::vec3(positions[i],	h2, nextZPos);
		glm::vec3 p3 = glm::vec3(prevXPos,		h3, nextZPos);
		glm::vec3 p4 = glm::vec3(prevXPos,		h4, positions[k]);
		glm::vec3 p5 = glm::vec3(positions[i],	h5, prevZPos);
		glm::vec3 p6 = glm::vec3(nextXPos,		h6, prevZPos);
		

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
	}
}
float TerrainChunk::generateTotalNoise(float xPos, float zPos)
{
	float totalNoise = 0.0f;

	totalNoise += 100 * Terrain::noiseGenerator->noise(xPos, zPos, 0.001f, NULL);
	totalNoise += 70 * Terrain::noiseGenerator->noise(xPos, zPos, 0.003f, NULL);
	totalNoise += 10 * Terrain::noiseGenerator->noise(xPos, zPos, 0.01f, NULL);
	totalNoise += 3 * Terrain::noiseGenerator->noise(xPos, zPos, 0.02f, NULL);

	return totalNoise;
}
#pragma endregion


