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

VAOData* VAOLoader::loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords)
{
	//Create and bind vao
	int vaoID = createVAO();

	//fill buffer for vertex positions
	storeVertexDataInAttributeList(0, positions);
	//fill buffer for vertex normals
	//Same layout, different attribute number
	storeVertexDataInAttributeList(2, normals);	

	//fill buffer for indices
	storeIndicesDataInAttributeList(3, indices);
	//fill buffer for texture coords
	storeTexCoordsInAttributeList(4, texCoords);



	unbindVAO();

	return new VAOData(vaoID, positions.size(), normals.size(), indices.size(), texCoords.size());
}

VAOData * VAOLoader::loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords, std::vector<float> scores)
{
	//Create and bind vao
	int vaoID = createVAO();

	//fill buffer for vertex positions
	storeVertexDataInAttributeList(0, positions);
	//fill buffer for vertex normals
	//Same layout, different attribute number
	storeVertexDataInAttributeList(2, normals);

	//fill buffer for indices
	storeIndicesDataInAttributeList(3, indices);
	//fill buffer for texture coords
	storeTexCoordsInAttributeList(4, texCoords);

	//store scores
	storeScoresInAttributeList(5, scores);

	unbindVAO();

	return new VAOData(vaoID, positions.size(), normals.size(), indices.size(), texCoords.size(), scores.size());
}

int VAOLoader::createVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);

	glBindVertexArray(vaoID);

	return vaoID;
}

void VAOLoader::storeVertexDataInAttributeList(int attributeNumber, std::vector<float> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(attributeNumber);

}


void VAOLoader::storeIndicesDataInAttributeList(int attributeNumber, std::vector<GLuint> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(GLuint), data.data(), GL_STATIC_DRAW);

}

void VAOLoader::storeTexCoordsInAttributeList(int attributeNumber, std::vector<float> data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 2, GL_FLOAT, false, 0, 0);
	glEnableVertexAttribArray(attributeNumber);


}

void VAOLoader::storeScoresInAttributeList(int attributeNumber, std::vector<float> data)
{

	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attributeNumber);
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

