#pragma once
#include <GL/gl3w.h>

#include <vector>

#include "VAOData.h"
class Renderer
{

public:

	void render(VAOData* vaoData, std::vector<GLuint>& textureIDs);

	GLuint shaderProgram;

	//Default
	GLenum drawMode = GL_TRIANGLES;

};

