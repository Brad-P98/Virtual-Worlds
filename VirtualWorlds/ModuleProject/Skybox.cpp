#include "Skybox.h"

Skybox::Skybox(const std::string skyboxFolderName, GLuint shader)
{
	m_shader = shader;

	fillFaceFilepaths(skyboxFolderName);

	generateVAO();

	loadCubeMap();

}

Skybox::~Skybox()
{
	glDeleteBuffers(1, &m_skyboxVBO);
	glDeleteVertexArrays(1, &m_skyboxVAO);
}



void Skybox::fillFaceFilepaths(const std::string skyboxFolderName)
{
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/front.png");
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/back.png");
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/top.png");
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/bottom.png");
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/right.png");
	faceFilePaths.push_back("Assets/" + skyboxFolderName + "/left.png");
}


void Skybox::generateVAO()
{

	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);

	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

}


void Skybox::loadCubeMap()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

	int width, height, numChannels;
	for (int i = 0; i < faceFilePaths.size(); i++) {
		unsigned char *data = stbi_load(faceFilePaths[i].c_str(), &width, &height, &numChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex failed to load at path: " << faceFilePaths[i] << std::endl;
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}




void Skybox::draw()
{
	glUseProgram(m_shader);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	glBindVertexArray(m_skyboxVAO);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}