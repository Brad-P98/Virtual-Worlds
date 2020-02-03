#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Object3D.h>
#include <VAOLoader.h>


class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, GLuint shader);
	~TerrainChunk();


private:
	void generateVertices();
	void generateIndices();

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;

private:

	float x;						//World position
	float z;						//"
	const float SIZE = 100;			//Size in world coords
	const int VERTEX_COUNT = 64;	//Number of vertices per side of terrain chunk

};

