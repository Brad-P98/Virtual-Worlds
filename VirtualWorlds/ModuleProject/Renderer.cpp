#include "Renderer.h"

void Renderer::render(VAOData vaoData)
{
	glUseProgram(shaderProgram);
	glCullFace(GL_NONE);
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(vaoData.getVaoID());
	glDrawArrays(GL_TRIANGLES, 0, vaoData.getVertexCount());
	glBindVertexArray(0);
}
