#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Object3D.h>
#include <VAOLoader.h>

#include "NoiseGenerator.h"

#include "SettlementManager.h"
#include "Building.h"


#pragma region Terrain Chunk
class TerrainChunk : public Object3D
{
public:

	TerrainChunk(int gridX, int gridZ, NoiseGenerator* noiseInterface, GLuint shader);
	~TerrainChunk();

	//Generate the VAO with the vertex data already worked out
	void generateVAO();

	std::vector<glm::vec3> generateSettlementPositions(int stride);
	std::vector<glm::vec3> generateSettlementPositionsProxBased(int stride);

	//Method to recalculate vertex scores for the whole chunk
	void resampleVertexScores(bool settlementsOnly, int vertexStride);

private:

	void generateUniqueVertexPositions();

	//SCORE CALCULATION
	//1. SampleGradient
	//The steeper the gradient, the fewer points awarded. Check only immediately surrounding vertices.
	float calcGradientScore(float xPos, float zPos, glm::vec3 vertexNormal);

	//2. SampleAltitude
	//Any vertices below sea level are immediately 0.
	//Vertices at a particularly high altitude are also less desirable.
	float calcAltitudeScore(float xPos, float zPos);

	//3. SampleSettlementProximity
	//Every settlement has a 'focal point' vertex, as well as a value suggesting the size of the settlement. This method iterates through nearby focal points, giving a score based on proximity to other settlements.
	float calcSettlementProxScore(glm::vec3 position);


	
	//Updates the score VBO with the current data stored in scores.
	//Call this method to update scores at any time after original VAO is already generated.
	void updateScoresVBO();


public:

	glm::vec3 chunkMinXZ;	//in world coords, the lowest x and lowest z vertex position
	int m_gridX;
	int m_gridZ;

	//score of every vertex, based only on terrain factors when pushed into VAO.
	std::vector<float> terrainScores;
	//Score of every vertex, based only on settlement influence factors.
	std::vector<float> settlementInfluenceScores;

	bool settlementsGenerated = false;

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
