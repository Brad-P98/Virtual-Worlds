#include "Object3D.h"

Object3D::ModelBuffer Object3D::modelBuffer;

void Object3D::init(VAOData* vaoData, GLuint textureID, GLuint shader)
{
	m_VaoData = vaoData;

	setShaderProgram(shader);

	m_TextureID = textureID;

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

void Object3D::draw()
{
	glUseProgram(m_Renderer.shaderProgram);
	m_Renderer.render(m_VaoData, m_TextureID);
}

void Object3D::setShaderProgram(GLuint shader)
{
	m_Renderer.shaderProgram = shader;
}


