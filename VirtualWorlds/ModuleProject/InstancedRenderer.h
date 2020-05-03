#pragma once
#include <GL/gl3w.h>

#include <vector>

#include "VAOData.h"
class InstancedRenderer
{
public:

	void render(VAOData* vaaoData, int instanceCount, std::vector<GLuint> textureIDs);

	GLuint shaderProgram;

	GLenum drawMode = GL_TRIANGLES;
};

