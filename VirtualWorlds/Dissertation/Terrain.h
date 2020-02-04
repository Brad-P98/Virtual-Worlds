#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Instance.h>
#include <Object3D.h>
#include <VAOLoader.h>


class TerrainChunk;

class Terrain {


public:

	Terrain();
	~Terrain();

public:

	//Takes in true for +, false for -
	void generateRow(bool sign);
	void generateCol(bool sign);

	void removeRow(bool sign);
	void removeCol(bool sign);

private:

	//10 chunks generated in each direction from camera
	const float RENDER_DISTANCE_CHUNKS = 10;
	std::vector<std::vector<TerrainChunk*>> activeTerrainChunks;

};

class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, GLuint shader);
	~TerrainChunk();


private:
	void generateVertices();
	void generateIndices();

public:

	glm::vec3 chunkCentre;			//in world coords

	const static float SIZE;		//Size in world coords
	const static int VERTEX_COUNT;	//Number of vertices per side of terrain chunk

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;

private:

	float x;						//World position
	float z;						//"


};

