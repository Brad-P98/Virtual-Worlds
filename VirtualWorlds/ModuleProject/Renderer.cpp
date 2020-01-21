#include "Renderer.h"

void Renderer::render(VAOData vaoData)
{
	glCullFace(GL_NONE);
	glBindVertexArray(vaoData.getVaoID());
	glDrawArrays(GL_TRIANGLES, 0, vaoData.getVertexCount());
	glBindVertexArray(0);
}
