#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Instance.h>
#include <Object3D.h>
#include <VAOLoader.h>

#include <TextureManager.h>

class WaterChunk;

class WaterPlane {

public:

	WaterPlane();
	~WaterPlane();

	void generateInitChunks(glm::vec3 startChunkGridPos);

	void adjustXRow(bool direction);
	void adjustZRow(bool direction);

	//Add chunks to scene from the temp vectors
	void finalizeXGeneration();
	void finalizeZGeneration();

private:

	void init();
	void generateVertexData();

public:

	const int RENDER_DISTANCE_CHUNKS = 10;
	static const float SEA_LEVEL;

	GLuint shader;

	static std::vector<float> vertexPositions;
	static std::vector<float> vertexNormals;
	static std::vector<GLuint> vertexIndices;
	static std::vector<float> textureCoords;

public:

	bool doneGeneratingX = false;
	bool doneGeneratingZ = false;

	bool idleX = true;
	bool idleZ = true;

private:

	std::vector<std::vector<WaterChunk*>> activeWaterChunks;

	//temp vectors storing the chunks that need to be added to the scene once finished generating
	std::vector<WaterChunk*> chunksToAddX;
	std::vector<WaterChunk*> chunksToAddZ;
	std::vector<WaterChunk*> chunksToRemoveX;
	std::vector<WaterChunk*> chunksToRemoveZ;
};



#pragma region Water Chunk
class WaterChunk : public Object3D {

public:
	WaterChunk(int gridX, int gridZ, GLuint shader);
	~WaterChunk();

	void generateVAO();

	void generateUniqueVertexPositions();

public:

	glm::vec3 chunkMinXZ;
	int m_gridX;
	int m_gridZ;

	const static float SIZE;

private:

	std::vector<GLuint> indices;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> texCoords;

private:

	GLuint m_shader;
	
	float x;
	float y;
	float z;
};




#pragma endregion