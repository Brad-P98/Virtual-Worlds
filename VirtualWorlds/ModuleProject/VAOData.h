#pragma once


class VAOData
{
public:

	VAOData(int vaoID, int vertexCount, int indexCount);
	~VAOData();

	int getVaoID() { return m_vaoID; }
	int getVertexCount() { return m_vertexCount; }
	int getIndexCount() { return m_indexCount; }

private:

	//Member variables
	int m_vaoID;
	int m_vertexCount;
	int m_indexCount;

};

