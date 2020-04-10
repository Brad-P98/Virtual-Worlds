#include "VAOLoader.h"

VAOLoader * VAOLoader::thisPointer = nullptr;
std::vector<GLuint> VAOLoader::vaos;
std::vector<GLuint> VAOLoader::vbos;

VAOLoader * VAOLoader::getInstance()
{
	if (thisPointer == nullptr)
	{
		thisPointer = new VAOLoader();
	}
	return thisPointer;
}

VAOLoader::VAOLoader()
{
}

//Generic indexed objects can use this.
VAOData* VAOLoader::loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords)
{
	//Create and bind vao
	int vaoID = createVAO();

	storeFloatDataInAttributeList(0, 3, positions);

	storeFloatDataInAttributeList(2, 3, normals);

	storeIndicesDataInAttributeList(3, indices);

	storeFloatDataInAttributeList(4, 2, texCoords);

	unbindVAO();

	return new VAOData(vaoID, positions.size(), normals.size(), indices.size(), texCoords.size());
}

//For terrain chunks specifically, as they have scores per vertex.
VAOData * VAOLoader::loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords, std::vector<float> scores)
{
	//Create and bind vao
	int vaoID = createVAO();

	//fill buffer for vertex positions. index 0, 3 components.
	storeFloatDataInAttributeList(0, 3, positions);

	//fill buffer for vertex normals
	//Same layout, different attribute number
	storeFloatDataInAttributeList(2, 3, normals);

	//fill buffer for indices
	storeIndicesDataInAttributeList(3, indices);
	//fill buffer for texture coords
	storeFloatDataInAttributeList(4, 2, texCoords);

	//store scores
	GLuint scoreVBO = storeFloatDataInAttributeList(5, 1, scores);

	//Create the new VAOData;
	VAOData* vaoData = new VAOData(vaoID, positions.size(), normals.size(), indices.size(), texCoords.size(), scores.size());

	unbindVAO();

	//include the score VBO ID in VAOData - lazy.
	vaoData->scoreVBOID = scoreVBO;

	return vaoData;
}

int VAOLoader::createVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);

	glBindVertexArray(vaoID);

	return vaoID;
}

//Store a vector of floats at index of attributeNumber, with n components per vertex.
GLuint VAOLoader::storeFloatDataInAttributeList(int attributeNumber, int numOfComponents, std::vector<float> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, numOfComponents, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(attributeNumber);

	return vboID;
}

//Element array GLuints. 1 component.
void VAOLoader::storeIndicesDataInAttributeList(int attributeNumber, std::vector<GLuint> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);

}


void VAOLoader::updateVBOInVAO(GLuint vaoID, GLuint vboID, int attributeNumber, int numOfComponents, std::vector<float> data)
{
	//Bind VAO
	glBindVertexArray(vaoID);

	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	//Write VBO with new vector data
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, numOfComponents, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(attributeNumber);

	unbindVAO();
}

void VAOLoader::unbindVAO()
{
	glBindVertexArray(0);
}

void VAOLoader::cleanUp()
{
	for (GLuint vao : vaos) {
		glDeleteVertexArrays(1, &vao);
	}

	for (GLuint vbo : vbos) {
		glDeleteBuffers(1, &vbo);
	}
}

