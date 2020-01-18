#pragma once
#include <GL/gl3w.h>
#include <GL/freeglut.h>
#include <vector>

#include "VAOData.h"

class VAOLoader
{
public:

	VAOData* loadToVAO(float* positions);

private:

	std::vector<GLuint> vaos;
	std::vector<GLuint> vbos;

	int createVAO();

	void storeDataInAttributeList(int attributeNumber, float* data);

	void unbindVAO();

	void cleanUp();
};

