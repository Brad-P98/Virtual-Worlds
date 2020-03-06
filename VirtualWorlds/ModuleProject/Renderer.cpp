#include "Renderer.h"

void Renderer::render(VAOData* vaoData, GLuint textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLuint vaoID = vaoData->getVaoID();
	glBindVertexArray(vaoData->getVaoID());
	glDrawElements(GL_TRIANGLES, vaoData->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
