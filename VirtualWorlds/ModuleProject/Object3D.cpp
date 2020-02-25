#include "Object3D.h"


void Object3D::init(VAOData* vaoData, GLuint shader)
{
	m_VaoData = vaoData;

	setShaderProgram(shader);
	//initialize with identity matrix
	transform = glm::mat4(1.0f);
	initTransformUBO();

	onInit();
}

void Object3D::initTransformUBO()
{

	//Escape function if a shader has been set that does not have a model ubo
	if (glGetUniformBlockIndex(getShaderProgram(), "Model") == GL_INVALID_INDEX) {
		std::cout << "Object's active shader program has no UBO named 'Model'" << std::endl;
		return;
	}

	unsigned int uniformBlockIndex = glGetUniformBlockIndex(getShaderProgram(), "Model");
	glUniformBlockBinding(getShaderProgram(), uniformBlockIndex, 1);

	//Setup UBO
	glGenBuffers(1, &transformUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, transformUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, transformUBO, 0, sizeof(glm::mat4));

	//Fill UBO with transform matrix (default as identity)
	glBindBuffer(GL_UNIFORM_BUFFER, transformUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(transform));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Object3D::onInit()
{
}

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
}

void Object3D::update()
{
	//perform derived object specific updates
	onUpdate();

	//Perform required updates for any object3d
	//update UBOs etc..
	updateTransformUBO();

}

void Object3D::onUpdate()
{
}

void Object3D::updateTransformUBO()
{
	//GLuint loc = glGetUniformLocation(getShaderProgram(), "yOffset");
	//glUniform1f(loc, sin((float)Clock::currTime * 0.001f));

	glBindBuffer(GL_UNIFORM_BUFFER, transformUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(transform));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Object3D::draw()
{
	glUseProgram(m_Renderer.shaderProgram);
	m_Renderer.render(*m_VaoData);
}

void Object3D::setShaderProgram(GLuint shader)
{
	m_Renderer.shaderProgram = shader;
}


