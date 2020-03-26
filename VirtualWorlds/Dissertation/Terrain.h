#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Instance.h>
#include <Object3D.h>
#include <VAOLoader.h>
#include <PerlinNoise.hpp>

#include <TextureManager.h>


class TerrainChunk;

class Terrain {


public:

	Terrain();
	~Terrain();

	void generateInitChunks(glm::vec3 startChunkGridPos);

	void generateXRow(glm::vec3 currentChunkPos, int rowIndex);
	void finalizeXRow(int rowIndex);


	void adjustXRow(bool direction);
	void adjustZRow(bool direction);

	//Add chunks to scene from the temp vectors
	void finalizeXGeneration();
	void finalizeZGeneration();

private:

	void init();

	//No need for default normals, as every normal is new per terrain chunk, so defaults serve no purpose
	//Generate the details for a plain terrain chunk
	void generateDefaultVertexPositions();
	void generateDefaultVertexIndices();

	//Named default but not intending to change these later
	void generateDefaultTextureCoords();

public:


	GLuint shader;

	//No need for default normals, as every normal is new per terrain chunk, so defaults serve no purpose
	static std::vector<float> defaultVertexPositions;
	static std::vector<GLuint> defaultVertexIndices;
	static std::vector<float> defaultTextureCoords;

public:

	bool doneGeneratingX = false;
	bool doneGeneratingZ = false;

	//idle when not working on generating terrain
	bool idleX = true;
	bool idleZ = true;

private:

	std::vector<std::vector<TerrainChunk*>> activeTerrainChunks;

	//temp vectors storing the chunks that need to be added to the scene once finished generating
	std::vector<TerrainChunk*> chunksToAddX;
	std::vector<TerrainChunk*> chunksToAddZ;
	std::vector<TerrainChunk*> chunksToRemoveX;
	std::vector<TerrainChunk*> chunksToRemoveZ;
};


#pragma region Terrain Chunk
class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, GLuint shader);
	~TerrainChunk();

	//Generate the VAO with the vertex data already worked out
	void generateVAO();

private:

	void generateUniqueVertexPositions();

public:

	glm::vec3 chunkMinXZ;	//in world coords, the lowest x and lowest z vertex position
	int m_gridX;
	int m_gridZ;

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;

	//settlement score of every vertex.
	std::vector<int> score;

private:

	GLuint m_shader;

	float x;						//World position
	float y;						//"
	float z;						//"

};

#pragma endregion

