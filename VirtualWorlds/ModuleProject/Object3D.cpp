#include "Object3D.h"


void Object3D::init(VAOData* vaoData)
{
	m_VaoData = vaoData;
	m_Renderer = new Renderer();
}

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
}

void Object3D::onInit()
{
}

void Object3D::update()
{
}

void Object3D::draw()
{
	glUseProgram(m_Renderer->shaderProgram);
	m_Renderer->render(*m_VaoData);
}

void Object3D::setShaderProgram(GLuint shader)
{
	m_Renderer->shaderProgram = shader;
}


