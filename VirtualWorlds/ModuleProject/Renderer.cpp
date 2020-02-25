#include "Renderer.h"

void Renderer::render(VAOData* vaoData)
{
	GLuint vaoID = vaoData->getVaoID();
	glBindVertexArray(vaoData->getVaoID());
	glDrawElements(GL_TRIANGLES, vaoData->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
