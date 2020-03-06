#include "VAOData.h"

VAOData::VAOData(int vaoID, int vertexCount, int normalCount, int indexCount, int texCoordCount)
{
	m_vaoID = vaoID;
	m_vertexCount = vertexCount;
	m_normalCount = normalCount;
	m_indexCount = indexCount;
	m_texCoordCount = texCoordCount;
}

VAOData::~VAOData()
{
	//GLuint temp = m_vaoID;
	//std::cout << "deleting VAO " << temp << std::endl;
	//glDeleteBuffers(1, &temp);
}
