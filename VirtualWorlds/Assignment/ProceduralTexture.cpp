#include "ProceduralTexture.h"
#include <random>

#define PI 3.14159265359

ProceduralTexture::ProceduralTexture()
{

	// Setup aligned data structures to store the uniform buffer data to pass to the shader
	struct alignas(16) aligned16_uint
	{
		GLuint value;
	};

	struct alignas(16) aligned16_vec2
	{
		glm::vec2 value;
	};

	struct alignas(16) PerlinModel
	{

		alignas(4) GLuint N = 0x100;
		alignas(4) GLuint Nmask = 0xFF;
		alignas(16) aligned16_uint iTable[256];
		alignas(16) aligned16_vec2 vTable[256];

	};

	PerlinModel* perlinNoiseModel = new PerlinModel();

	perlinNoiseModel->N = 256;
	perlinNoiseModel->Nmask = 255;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());
	std::uniform_int_distribution<GLuint> indexDist(0, 255);

	float theta = 0.0f;
	float pi2 =  PI * 2.0f;
	float angleStep = pi2 / (float)perlinNoiseModel->N;

	for (GLuint i = 0; i < 256; i++) {

		perlinNoiseModel->iTable[i].value = indexDist(mt);

		// unit length gradient vector
		perlinNoiseModel->vTable[i].value.x = cos(theta);
		perlinNoiseModel->vTable[i].value.y = sin(theta);

		theta += angleStep;
	}

	// Setup buffer object for perlin model
	glCreateBuffers(1, &perlinModelBuffer);
	glNamedBufferStorage(perlinModelBuffer, sizeof(PerlinModel), perlinNoiseModel, 0);

}


ProceduralTexture::~ProceduralTexture()
{
}



GLuint ProceduralTexture::generateTexture(GLuint w, GLuint h)
{

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Setup texture data - but pass null for last parameter.  OpenGL creates the texture image array in memory but no data is passed into this.  We'll provide the content when we run our shader.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	// Setup texture properties
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Bind compute shader
	glUseProgram(m_Shader);

	// Bind the perlin uniform buffer
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, perlinModelBuffer);
	// Bind the texture to image object binding point 0
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

	GLuint numGroupsX = w >> 4;
	GLuint numGroupsY = h >> 4;
	GLuint numGroupsZ = 1;

	// Run our compute shader to fill in the texture data
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);

	glBindTexture(GL_TEXTURE_2D, 0);

	//Texture is generated, so return id so it can be added to the texture manager
	return texture;
}
