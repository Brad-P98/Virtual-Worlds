#include "Terrain.h"
#include "WaterPlane.h"
#include "ChunkSettings.h"
#include "TerrainNoise.h"

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
	//Initialize TerrainNoise details
	TerrainNoise::noiseGenerator = new PerlinNoise();
	//Add noise layers to an easily accessible struct.
	TerrainNoise::layers.push_back(new NoiseLayer(100, 0.0004f));
	TerrainNoise::layers.push_back(new NoiseLayer(70, 0.002f));
	TerrainNoise::layers.push_back(new NoiseLayer(10, 0.01f));
	TerrainNoise::layers.push_back(new NoiseLayer(3, 0.02f));


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

		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, shader);

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
		
		TerrainChunk* newChunk = new TerrainChunk(xPos, zStart + i, shader);

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

			//Instance::m_scene->removeObject(activeTerrainChunks[i][0]);
			chunksToRemoveZ.push_back(activeTerrainChunks[i][0]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin());
		}
	}
	else {
		for (int i = 0; i < activeTerrainChunks.size(); i++) {

			//Instance::m_scene->removeObject(activeTerrainChunks[i][2*RENDER_DISTANCE_CHUNKS]);
			chunksToRemoveZ.push_back(activeTerrainChunks[i][2 * renderDistance]);

			activeTerrainChunks[i].erase(activeTerrainChunks[i].begin() + 2*renderDistance);
		}
	}

	//Create all new chunks and push into vector
	for (int i = 0; i < 2 * renderDistance + 1; i++) {
	
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

	init(loader->loadToVAO(positions, normals, indices, texCoords), texID, m_shader);
}

void TerrainChunk::generateUniqueVertexPositions()
{
	for (int i = 0, j = 1, k = 2; i < positions.size(); i+=3, j+=3, k+=3) {
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
#pragma endregion


