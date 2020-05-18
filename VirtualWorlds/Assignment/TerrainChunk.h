#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Object3D.h>
#include <VAOLoader.h>

#include "NoiseGenerator.h"



#pragma region Terrain Chunk
class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, NoiseGenerator* noiseInterface, GLuint shader);
	~TerrainChunk();

	//Generate the VAO with the vertex data already worked out
	void generateVAO();


	//Method to recalculate vertex scores for the whole chunk
	void resampleVertexScores(bool settlementsOnly, int vertexStride);

	void generateSurfaceDetail();

private:

	void generateUniqueVertexPositions();

	void tryGenRock(glm::vec3 pos, float chance);

	float calcGradientScore(float xPos, float zPos, glm::vec3 vertexNormal);

	float calcAltitudeScore(float xPos, float zPos);


	
	//Updates the score VBO with the current data stored in scores.
	//Call this method to update scores at any time after original VAO is already generated.
	void updateScoresVBO();


public:

	glm::vec3 chunkMinXZ;	//in world coords, the lowest x and lowest z vertex position
	int m_gridX;
	int m_gridZ;

	//score of every vertex, based only on terrain factors when pushed into VAO.
	std::vector<float> terrainScores;

	bool surfaceDetailGenerated = false;
private:

	std::vector<GLuint> indices;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;



private:

	GLuint m_shader;
	NoiseGenerator* m_noiseInterface;

	float x;						//World position
	float y;						//"
	float z;						//"
};

#pragma endregion
