#include "Terrain.h"



Terrain::Terrain(int width, int length, GLuint shader)
{
	
	//generateTerrain(width, length);
	generateVertices(width, length);
	VAOLoader* loader =  new VAOLoader();
	init(loader->loadToVAO(positions), shader);
}


Terrain::~Terrain()
{
}

void Terrain::generateTerrain(int width, int length)
{

	generateVertices(width, length);
	generateIndices(width, length);

}

void Terrain::generateVertices(int width, int length)
{
	//length first
	for (int z = 0; z < width; z++) {

		for (int x = 0; x < length; x++) {

			positions.push_back(x);
			positions.push_back(0.0f);
			positions.push_back(z);
		}
	}
}

void Terrain::generateIndices(int width, int length) 
{
	for (int i = 0; i < positions.size(); i++) {

		//2 triangles indexed per loop
		indices.push_back(i);
		indices.push_back(i + width);
		indices.push_back(i + 1);
		indices.push_back(i + 1);
		indices.push_back(i + width);
		indices.push_back(i + width + 1);
	}
}