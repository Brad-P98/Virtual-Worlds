#include "Renderer.h"

void Renderer::render(VAOData vaoData)
{
	glBindVertexArray(vaoData.getVaoID());
	glDrawArrays(GL_TRIANGLES, 0, vaoData.getVertexCount());
	glBindVertexArray(0);
}
