#pragma once
#include <GL/gl3w.h>
#include <iostream>

class VAOData
{
public:

	VAOData(int vaoID, int vertexCount, int normalCount, int indexCount, int texCoordCount);

	//Optional, for terrain.
	VAOData(int vaoID, int vertexCount, int normalCount, int indexCount, int texCoordCount, int score);

	~VAOData();

	int getVaoID() const { return m_vaoID; }
	int getVertexCount() const { return m_vertexCount; }
	int getNormalCount() const { return m_normalCount; }
	int getIndexCount() const { return m_indexCount; }
	int getTexCoordCount() const { return m_texCoordCount; }
	
	int getScoreCount() const { return m_scoreCount; }

	GLuint scoreVBOID;
private:

	//Member variables
	int m_vaoID;
	int m_vertexCount;
	int m_normalCount;
	int m_indexCount;
	int m_texCoordCount;

	//terrain optional
	int m_scoreCount;


};

