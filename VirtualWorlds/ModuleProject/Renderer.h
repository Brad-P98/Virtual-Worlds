#pragma once
#include <GL/gl3w.h>

#include "VAOData.h"
class Renderer
{

public:

	void render(VAOData* vaoData, GLuint textureID);

	GLuint shaderProgram;

};

