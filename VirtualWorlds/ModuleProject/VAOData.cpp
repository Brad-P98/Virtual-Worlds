#include "VAOData.h"

VAOData::VAOData(int vaoID, int vertexCount, int indexCount)
{
	m_vaoID = vaoID;
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
	std::cout << "creating VAO " << m_vaoID << std::endl;

}

VAOData::~VAOData()
{
	GLuint temp = m_vaoID;
	std::cout << "deleting VAO " << temp << std::endl;
	glDeleteBuffers(1, &temp);
}
