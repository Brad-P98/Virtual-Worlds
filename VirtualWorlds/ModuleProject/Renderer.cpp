#include "Renderer.h"

void Renderer::render(VAOData vaoData)
{
	glBindVertexArray(vaoData.getVaoID());
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, vaoData.getVertexCount());
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
