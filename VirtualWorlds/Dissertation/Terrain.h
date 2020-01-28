#pragma once
#include <GL/gl3w.h>
#include <vector>

#include <Object3D.h>
#include <VAOLoader.h>

class Terrain : public Object3D
{
public:

	Terrain(int width, int length, GLuint shader);
	~Terrain();

	void generateTerrain(int width, int length);



private:
	void generateVertices(int width, int length);
	void generateIndices(int width, int length);

private:

	std::vector<float> indices;
	std::vector<float> positions;

private:

	VAOData* m_vaoData;
};

