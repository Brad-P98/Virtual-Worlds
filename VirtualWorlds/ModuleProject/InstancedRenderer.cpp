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

	glDrawElementsInstanced(drawMode, vaoData->getIndexCount(), GL_UNSIGNED_INT, (void*)0, instanceCount);
}
