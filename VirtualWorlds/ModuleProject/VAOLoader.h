#pragma once

#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <vector>

#include "VAOData.h"

class VAOLoader
{
public:

	VAOData* loadToVAO(std::vector<float> positions, std::vector<float> indices);

private:

	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;

	int createVAO();

	void storeDataInAttributeList(int attributeNumber, std::vector<float> data);

	void unbindVAO();

	void cleanUp();
};

