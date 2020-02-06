#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Instance.h>
#include <Object3D.h>
#include <VAOLoader.h>
#include <PerlinNoise.hpp>


class TerrainChunk;

class Terrain {


public:

	Terrain();
	~Terrain();

	void GenerateInitChunks(glm::vec3 startChunkGridPos);

private:

	void init();

	void GenerateDefaultVertexPositions();
	void GenerateDefaultVertexIndices();

public:
	//chunks generated in each direction from current chunk (excludes current chunk)
	const int RENDER_DISTANCE_CHUNKS = 6;

	static PerlinNoise* noiseGenerator;

	GLuint shader;

	static std::vector<float> defaultVertexPositions;
	static std::vector<GLuint> defaultVertexIndices;

private:

	std::vector<std::vector<TerrainChunk*>> activeTerrainChunks;

};

class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, GLuint shader);
	~TerrainChunk();


private:

	void generateUniqueVertexPositions();

public:

	glm::vec3 chunkCentre;			//in world coords

	const static float SIZE;		//Size in world coords
	const static int VERTEX_COUNT;	//Number of vertices per side of terrain chunk

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;

private:

	float x;						//World position
	float y;						//"
	float z;						//"

};

