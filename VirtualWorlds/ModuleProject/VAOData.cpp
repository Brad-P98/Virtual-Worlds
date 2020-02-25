#include "VAOData.h"

VAOData::VAOData(int vaoID, int vertexCount, int indexCount)
{
	m_vaoID = vaoID;
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;
}

VAOData::~VAOData()
{
	//GLuint temp = m_vaoID;
	//glDeleteBuffers(1, &m_vaoID);
}
