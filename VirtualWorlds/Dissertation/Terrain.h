#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Instance.h>
#include <Object3D.h>
#include <VAOLoader.h>
#include <PerlinNoise.hpp>

#include <TextureManager.h>

#include "TerrainChunk.h"


class TerrainChunk;
class TerrainSampler;

class Terrain {


public:

	Terrain();
	~Terrain();

	void generateXRow(glm::vec3 currentChunkPos, int rowIndex);
	void finalizeXRow(int rowIndex);


	void adjustXRow(bool direction);
	void adjustZRow(bool direction);

	//Add chunks to scene from the temp vectors
	void finalizeXGeneration();
	void finalizeZGeneration();

	std::vector<std::vector<TerrainChunk*>> getActiveTerrainChunks() const { return activeTerrainChunks; }

	float getYAtPosition(float x, float z);

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
	NoiseGenerator* noiseInterface;

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