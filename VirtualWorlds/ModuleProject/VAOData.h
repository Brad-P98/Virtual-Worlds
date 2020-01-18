#pragma once


class VAOData
{
public:

	VAOData(int vaoID, int vertexCount);
	~VAOData();

	int getVaoID() { return m_vaoID; }
	int getVertexCount() { return m_vertexCount; }

private:

	//Member variables
	int m_vaoID;
	int m_vertexCount;

};

