#include "Renderer.h"

void Renderer::render(VAOData* vaoData, std::vector<GLuint> textureIDs)
{
	//Bind all textures

	//Max. 5 textures, for now
	for (int i = 0; i < textureIDs.size() && i < 5; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureIDs[i]);
	}


	GLuint vaoID = vaoData->getVaoID();
	glBindVertexArray(vaoData->getVaoID());
	glDrawElements(drawMode, vaoData->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}
