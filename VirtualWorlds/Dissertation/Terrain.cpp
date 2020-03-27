#include "Terrain.h"
#include "WaterPlane.h"
#include "ChunkSettings.h"

//Constants used for generating chunks. Predefined to clean up code.
#define chunkSize ChunkSettings::CHUNK_SIZE
#define vertexCount ChunkSettings::CHUNK_EDGE_VERTEX_COUNT
#define renderDistance ChunkSettings::CHUNK_RENDER_DISTANCE

std::vector<float> Terrain::defaultVertexPositions;
std::vector<GLuint> Terrain::defaultVertexIndices;
std::vector<float> Terrain::defaultTextureCoords;



#pragma region Terrain
Terrain::Terrain()
{
	init();

	generateDefaultVertexPositions();
	generateDefaultVertexIndices();
	generateDefaultTextureCoords();
}

Terrain::~Terrain()
{
}

void Terrain::init()
{
	noiseInterface = new NoiseGenerator();
	//Initialize TerrainNoise details
	noiseInterface->noiseGenerator = new PerlinNoise();
	//Add noise layers to an easily accessible struct.
	noiseInterface->layers.push_back(new NoiseLayer(100, 0.0004f));
	noiseInterface->layers.push_back(new NoiseLayer(70, 0.002f));
	noiseInterface->layers.push_back(new NoiseLayer(10, 0.01f));
	noiseInterface->layers.push_back(new NoiseLayer(3, 0.02f));

	//Allocate space for all terrain chunks
	activeTerrainChunks.resize(renderDistance * 2 + 1, std::vector<TerrainChunk*>(renderDistance * 2 + 1, nullptr));
}

//X and Z are always the same for every terrain chunk
void Terrain::generateDefaultVertexPositions()
{
	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {
			//x
			defaultVertexPositions.push_back((float)j / ((float)vertexCount - 1) * chunkSize);
			//y
			defaultVertexPositions.push_back(0.0f);
			//z
			defaultVertexPositions.push_back((float)i / ((float)vertexCount - 1) * chunkSize);
		}
	}
}

//indices are always the same for every terrain chunk
void Terrain::generateDefaultVertexIndices()
{
	for (int i = 0; i < vertexCount - 1; i++) {
		for (int j = 0; j < vertexCount - 1; j++) {

			int topLeft = (i*vertexCount) + j;
			int topRight = topLeft + 1;
			int bottomLeft = ((i + 1) * vertexCount) + j;
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

//Texture coords are always the same for every terrain chunk
void Terrain::generateDefaultTextureCoords()
{
	float vertexSeperationDist = chunkSize / vertexCount;

	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {
			
			defaultTextureCoords.push_back(((float)i / ((float)vertexCount - 1) * chunkSize) / 100); //x
			defaultTextureCoords.push_back(((float)j / ((float)vertexCount - 1) * chunkSize) / 100); //z
		}
	}

}


//Generate just one row of chunks, rowIndex distance away from current chunk in x axis.
//Does not consider any other chunks that may already be here, simply overwrites.
//if row == 0, generates a row on current position
void Terrain::generateXRow(glm::vec3 currentChunkPos, int row)
{
	//If outside render distance, do not generate chunks
	if (abs(row) > renderDistance) return;


	//x coord in the world chunk grid to generate chunks onto
	int xPos = currentChunkPos.x + row;

	//starting z coord in the world chunk grid.
	int zStart = currentChunkPos.z - renderDistance;

	//index of this row in the active chunk vector.
	int rowIndexInActiveChunks = row + renderDistance;

	//Create the new row of chunks
	for (int i = 0; i < 2 * renderDistance + 1; i++) {

		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, noiseInterface, shader);

		//Add chunk to the correct position in active chunks
		activeTerrainChunks[rowIndexInActiveChunks][i] = newChunk;
	}

}

//Called when row is finished generating on thread
void Terrain::finalizeXRow(int rowIndex)
{
	//iterate through Z chunks on one X row
	for (int i = 0; i < activeTerrainChunks[rowIndex].size(); i++) {

		//Generate and add to scene
		activeTerrainChunks[rowIndex][i]->generateVAO();
		Instance::m_scene->addObject(activeTerrainChunks[rowIndex][i]);
	}

}



//Methods for generating new rows at the render distance bounds when player moves.
//false = remove lowest x chunk, add higher x chunk
void Terrain::adjustXRow(bool direction)
{

	if (!idleX) return;
	idleX = false;
	//new row's x position in grid
	int xPos;
	if (!direction) {

		xPos = activeTerrainChunks[2 * renderDistance][0]->m_gridX + 1;
	}
	else {

		xPos = activeTerrainChunks[0][0]->m_gridX - 1;
	}


	//new row's starting z position in grid
	int zStart = activeTerrainChunks[2 * renderDistance][0]->m_gridZ;

	//Erase the correct row of chunks from the vector
	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeTerrainChunks[0].size(); i++) {

			chunksToRemoveX.push_back(activeTerrainChunks[0][i]);
		}
		//remove from vector
		activeTerrainChunks.erase(activeTerrainChunks.begin());
	}
	else {
		for (int i = 0; i < activeTerrainChunks[2 * renderDistance].size(); i++) {
			chunksToRemoveX.push_back(activeTerrainChunks[2 * renderDistance][i]);
		}
		activeTerrainChunks.erase(activeTerrainChunks.begin() + 2 * renderDistance);
	}

	//Create the new row of chunks
	std::vector<TerrainChunk*> tempChunkRow;
	for (int i = 0; i < 2 * renderDistance + 1; i++) {
		
		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, noiseInterface, shader);

		tempChunkRow.push_back(newChunk);
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
		zPos = activeTerrainChunks[0][2 * renderDistance]->m_gridZ + 1;
	}
	else {
		zPos = activeTerrainChunks[0][0]->m_gridZ - 1;

	}

	int xStart = activeTerrainChunks[0][0]->m_gridX;

	if (!direction) {
		//Remove from scene
		for (int i = 0; i < activeTerrainChunks.size(); i++) {
			chunksToRemoveZ.push_back(activeTerrainChunks[i][0]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin());
		}
	}
	else {
		for (int i = 0; i < activeTerrainChunks.size(); i++) {

			chunksToRemoveZ.push_back(activeTerrainChunks[i][2 * renderDistance]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin() + 2*renderDistance);
		}
	}

	//Create all new chunks and push into vector
	for (int i = 0; i < 2 * renderDistance + 1; i++) {
	
		TerrainChunk* newChunk = new TerrainChunk(xStart + i, zPos, noiseInterface, shader);

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