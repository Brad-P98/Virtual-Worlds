#include "InstancedRenderer.h"

void InstancedRenderer::render(VAOData * vaoData, int instanceCount, std::vector<GLuint> textureIDs)
{
	//Max. 5 textures, for now
	for (int i = 0; i < textureIDs.size() && i < 5; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
	}

	//Bind VAO
	glBindVertexArray(vaoData->getVaoID());
	enableVertexArrays(vaoData->getVaoID());

	glDrawElementsInstanced(drawMode, vaoData->getIndexCount(), GL_UNSIGNED_INT, (void*)0, instanceCount);

	disableVertexArrays(vaoData->getVaoID());

	//Unbind textures
	for (int i = 0; i < textureIDs.size() && i < 5; i++) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void InstancedRenderer::enableVertexArrays(GLuint vao)
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glEnableVertexAttribArray(8);
	glEnableVertexAttribArray(9);

}

void InstancedRenderer::disableVertexArrays(GLuint vao)
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
	glDisableVertexAttribArray(5);
	glDisableVertexAttribArray(6);
	glDisableVertexAttribArray(7);
	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(9);
}
