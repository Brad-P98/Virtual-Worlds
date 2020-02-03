#pragma once

#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <vector>

#include "VAOData.h"

class VAOLoader
{
public:

	VAOLoader();
	VAOData* loadToVAO(std::vector<float> positions, std::vector<GLuint> indices);

private:

	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;

	int createVAO();

	void storeVertexDataInAttributeList(int attributeNumber, std::vector<float> data);
	void storeIndicesDataInAttributeList(int attributeNumber, std::vector<GLuint> data);

	void unbindVAO();

	void cleanUp();
};

