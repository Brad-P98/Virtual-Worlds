#include "VAOLoader.h"

VAOData* VAOLoader::loadToVAO(float* positions)
{
	int vaoID = createVAO();

	storeDataInAttributeList(0, positions);
	unbindVAO();

	return new VAOData(vaoID, 9);
}

int VAOLoader::createVAO()
{
	GLuint vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);

	glBindVertexArray(vaoID);

	return vaoID;
}

void VAOLoader::storeDataInAttributeList(int attributeNumber, float* data)
{
	GLuint vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, false, 0, 0);
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

