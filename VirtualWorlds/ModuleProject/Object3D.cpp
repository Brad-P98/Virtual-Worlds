#include "Object3D.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object3D::ModelBuffer Object3D::modelBuffer;

void Object3D::init(VAOData* vaoData, const char* textureFilePath, GLuint shader)
{
	m_VaoData = vaoData;

	setShaderProgram(shader);

	genTextureBuffer(textureFilePath);

	//initialize with identity matrix
	transform = glm::mat4(1.0f);
	initModelUBO();

	onInit();
}

void Object3D::initModelUBO()
{

	//Escape function if a shader has been set that does not have a model ubo
	if (glGetUniformBlockIndex(getShaderProgram(), "Model") == GL_INVALID_INDEX) {
		std::cout << "Object's active shader program has no UBO named 'Model'" << std::endl;
		return;
	}

	unsigned int uniformBlockIndex = glGetUniformBlockIndex(getShaderProgram(), "Model");
	glUniformBlockBinding(getShaderProgram(), uniformBlockIndex, 1);

	//Setup UBO
	glGenBuffers(1, &modelUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, modelUBO, 0, sizeof(modelBuffer));

	updateModelUBO();
}

void Object3D::onInit()
{
}

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
	delete m_VaoData;
}

void Object3D::update()
{
	//perform derived object specific updates
	onUpdate();

	//Perform required updates for any object3d
	//update UBOs etc..
	updateModelUBO();

}

void Object3D::onUpdate()
{
}

void Object3D::updateModelUBO()
{
	//Fill struct with updated data
	modelBuffer.transform = transform;

	//Fill UBO with struct data
	glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(modelBuffer), &modelBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Object3D::genTextureBuffer(const char* filePath)
{

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, numChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &numChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Object3D::draw()
{
	glUseProgram(m_Renderer.shaderProgram);
	m_Renderer.render(m_VaoData, textureID);
}

void Object3D::setShaderProgram(GLuint shader)
{
	m_Renderer.shaderProgram = shader;
}


