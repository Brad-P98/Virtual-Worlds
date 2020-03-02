#pragma once

#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <glm/glm/glm.hpp>
#include <vector>

#include "VAOData.h"

class VAOLoader
{
public:


	static VAOData* loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices);

	static VAOLoader * getInstance();
private:

	static VAOLoader * thisPointer;


	VAOLoader();
	
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;


	static void storeVertexDataInAttributeList(int attributeNumber, std::vector<float> data);
	static void storeIndicesDataInAttributeList(int attributeNumber, std::vector<GLuint> data);
	static int createVAO();
	
	static void unbindVAO();
	 
	static void cleanUp();
};

