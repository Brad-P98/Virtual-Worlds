#include "WaterPlane.h"

const float WaterChunk::SIZE = 100;

const float WaterPlane::SEA_LEVEL = -20.0f;

std::vector<float> WaterPlane::vertexPositions;
std::vector<float> WaterPlane::vertexNormals;
std::vector<GLuint> WaterPlane::vertexIndices;
std::vector<float> WaterPlane::textureCoords;


#pragma region WaterPlane
WaterPlane::WaterPlane()
{
	init();

	generateVertexData();


}

WaterPlane::~WaterPlane()
{

}

void WaterPlane::init()
{

}

void WaterPlane::generateVertexData()
{
	//4 corners positions
	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(0.0f);

	vertexPositions.push_back(100.0f);
	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(0.0f);

	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(100.0f);

	vertexPositions.push_back(100.0f);
	vertexPositions.push_back(0.0f);
	vertexPositions.push_back(100.0f);

	//normals
	for (int i = 0; i < 4; i++) {
		vertexNormals.push_back(0.0f);
		vertexNormals.push_back(1.0f);
		vertexNormals.push_back(0.0f);
	}


	//indices
	vertexIndices.push_back(0);
	vertexIndices.push_back(1);
	vertexIndices.push_back(2);

	vertexIndices.push_back(1);
	vertexIndices.push_back(3);
	vertexIndices.push_back(2);

	//tex coords
	textureCoords.push_back(0.0f);
	textureCoords.push_back(0.0f);

	textureCoords.push_back(1.0f);
	textureCoords.push_back(0.0f);

	textureCoords.push_back(0.0f);
	textureCoords.push_back(1.0f);

	textureCoords.push_back(1.0f);
	textureCoords.push_back(1.0f);
}

void WaterPlane::generateInitChunks(glm::vec3 startChunkGridPos)
{
	activeWaterChunks.resize(RENDER_DISTANCE_CHUNKS * 2 + 1, std::vector<WaterChunk*>(RENDER_DISTANCE_CHUNKS * 2 + 1, nullptr));

	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {
		for (int j = 0; j < 2 * RENDER_DISTANCE_CHUNKS + 1; j++) {

			WaterChunk* newChunk = new WaterChunk(startChunkGridPos.x + i - RENDER_DISTANCE_CHUNKS, startChunkGridPos.z + j - RENDER_DISTANCE_CHUNKS, shader);

			newChunk->generateVAO();
			Instance::m_scene->addObject(newChunk);

			activeWaterChunks[i][j] = newChunk;
		}
	}
}



void WaterPlane::adjustXRow(bool direction) 
{
	if (!idleX) return;
	idleX = false;
	//new row's x position in grid
	int xPos;
	if (!direction) {

		xPos = activeWaterChunks[2 * RENDER_DISTANCE_CHUNKS][0]->m_gridX + 1;
	}
	else {

		xPos = activeWaterChunks[0][0]->m_gridX - 1;
	}

	//new row's starting z position in grid
	int zStart = activeWaterChunks[2 * RENDER_DISTANCE_CHUNKS][0]->m_gridZ;
	//Erase the correct row of chunks from the vector
	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeWaterChunks[0].size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[0][i]);
			chunksToRemoveX.push_back(activeWaterChunks[0][i]);
		}
		//remove from vector
		activeWaterChunks.erase(activeWaterChunks.begin());
	}
	else {
		for (int i = 0; i < activeWaterChunks[2 * RENDER_DISTANCE_CHUNKS].size(); i++) {

			chunksToRemoveX.push_back(activeWaterChunks[2 * RENDER_DISTANCE_CHUNKS][i]);
		}
		activeWaterChunks.erase(activeWaterChunks.begin() + 2 * RENDER_DISTANCE_CHUNKS);
	}

	//Create the new row of chunks
	std::vector<WaterChunk*> tempChunkRow;
	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {


		WaterChunk* newChunk = new WaterChunk(xPos, zStart + i, shader);

		tempChunkRow.push_back(newChunk);
		//Instance::m_scene->addObject(newChunk);
		chunksToAddX.push_back(newChunk);
	}
	//Add the row of chunks to the correct end of the vector
	if (!direction) {
		activeWaterChunks.push_back(tempChunkRow);
	}
	else {
		activeWaterChunks.insert(activeWaterChunks.begin(), tempChunkRow);
	}
	idleX = true;
	doneGeneratingX = true;
}

void WaterPlane::adjustZRow(bool direction)
{
	if (!idleZ) return;
	idleZ = false;
	int zPos;
	if (!direction) {
		zPos = activeWaterChunks[0][2 * RENDER_DISTANCE_CHUNKS]->m_gridZ + 1;
	}
	else {
		zPos = activeWaterChunks[0][0]->m_gridZ - 1;

	}

	int xStart = activeWaterChunks[0][0]->m_gridX;

	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeWaterChunks.size(); i++) {
			chunksToRemoveZ.push_back(activeWaterChunks[i][0]);

			activeWaterChunks[i].erase(activeWaterChunks[i].begin());
		}
	}
	else {
		for (int i = 0; i < activeWaterChunks.size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[i][2*RENDER_DISTANCE_CHUNKS]);
			chunksToRemoveZ.push_back(activeWaterChunks[i][2 * RENDER_DISTANCE_CHUNKS]);

			activeWaterChunks[i].erase(activeWaterChunks[i].begin() + 2 * RENDER_DISTANCE_CHUNKS);
		}
	}

	//Create all new chunks and push into vector
	for (int i = 0; i < 2 * RENDER_DISTANCE_CHUNKS + 1; i++) {

		WaterChunk* newChunk = new WaterChunk(xStart + i, zPos, shader);
		//Instance::m_scene->addObject(newChunk);

		chunksToAddZ.push_back(newChunk);

		if (!direction) {
			activeWaterChunks[i].push_back(newChunk);
		}
		else {
			activeWaterChunks[i].insert(activeWaterChunks[i].begin(), newChunk);
		}
	}
	idleZ = true;
	doneGeneratingZ = true;

}

void WaterPlane::finalizeXGeneration()
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

void WaterPlane::finalizeZGeneration()
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



#pragma region Water Chunk
WaterChunk::WaterChunk(int gridX, int gridZ, GLuint shader)
{
	m_gridX = gridX;
	m_gridZ = gridZ;

	m_shader = shader;

	x = gridX * SIZE;
	z = gridZ * SIZE;

	chunkMinXZ = glm::vec3(x, 0, z);

	//Default vertex data
	positions = WaterPlane::vertexPositions;
	normals = WaterPlane::vertexNormals;
	indices = WaterPlane::vertexIndices;
	texCoords = WaterPlane::textureCoords;

	generateUniqueVertexPositions();

}

WaterChunk::~WaterChunk()
{
}

void WaterChunk::generateVAO()
{
	VAOLoader* loader = VAOLoader::getInstance();

	GLuint texID = TextureManager::getTextureID("Assets/water.png");

	init(loader->loadToVAO(positions, normals, indices, texCoords), texID, m_shader);
}

void WaterChunk::generateUniqueVertexPositions()
{
	for (int i = 0, j = 1, k = 2; i < positions.size(); i += 3, j += 3, k += 3) {

		positions[i] += x;
		positions[j] += WaterPlane::SEA_LEVEL;
		positions[k] += z;


	}

}

#pragma endregion