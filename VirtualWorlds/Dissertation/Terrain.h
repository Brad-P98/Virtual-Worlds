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

	void generateInitChunks(glm::vec3 startChunkGridPos);

	void adjustXRow(bool direction);
	void adjustZRow(bool direction);

private:

	void init();

	void generateDefaultVertexPositions();
	void generateDefaultVertexNormals();
	void generateDefaultVertexIndices();

public:
	//chunks generated in each direction from current chunk (excludes current chunk)
	const int RENDER_DISTANCE_CHUNKS = 5;

	static PerlinNoise* noiseGenerator;

	GLuint shader;

	static std::vector<float> defaultVertexPositions;
	static std::vector<float> defaultVertexNormals;
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

	glm::vec3 chunkMinXZ;			//in world coords
	int m_gridX;
	int m_gridZ;

	const static float SIZE;		//Size in world coords
	const static int VERTEX_COUNT;	//Number of vertices per side of terrain chunk

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;
	std::vector<float> normals;

private:

	float x;						//World position
	float y;						//"
	float z;						//"

};

