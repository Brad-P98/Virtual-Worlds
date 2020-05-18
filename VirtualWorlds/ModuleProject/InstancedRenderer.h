#pragma once
#include <GL/gl3w.h>

#include <vector>

#include "VAOData.h"
class InstancedRenderer
{
public:

	void render(VAOData* vaoData, int instanceCount, std::vector<GLuint> textureIDs);

	GLuint shaderProgram;

	GLenum drawMode = GL_TRIANGLES;

private:

	void enableVertexArrays(GLuint vao);
	void disableVertexArrays(GLuint vao);
};

