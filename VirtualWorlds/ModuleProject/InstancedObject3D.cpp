#include "InstancedObject3D.h"


InstancedObject3D::InstancedObject3D()
{
}

InstancedObject3D::~InstancedObject3D()
{
	delete m_VaoData;
}



void InstancedObject3D::addInstance(StaticInstance* newModel)
{
	if (!(models.size() >= maxInstances)) return;
	newModel->id = instanceID;
	models.push_back(newModel);
	instanceID++;
}

void InstancedObject3D::removeInstance(int id)
{
	for (int i = 0; i < models.size(); i++) {
		if (models[i]->id == id) {
			//Remove model
			models.erase(models.begin() + i);
			return;
		}
	}
}




void InstancedObject3D::init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader)
{

	m_VaoData = vaoData;

	setShaderProgram(shader);

	m_TextureIDs = textureIDs;

	//Initialize size of vector
	models.resize(maxInstances);

	onInit();
}



void InstancedObject3D::update()
{
	onUpdate();

}

void InstancedObject3D::draw()
{

	//Use shader program
	glUseProgram(m_InstancedRenderer.shaderProgram);

	m_InstancedRenderer.render(m_VaoData, 0, m_TextureIDs);
}


void InstancedObject3D::onInit()
{
}

void InstancedObject3D::onUpdate()
{
	//Implemented by child. Child can iterate through instances and decide
	//whether something must be deleted or not, or more
}

void InstancedObject3D::setShaderProgram(GLuint shader)
{
	m_InstancedRenderer.shaderProgram = shader;
}

void InstancedObject3D::setDrawMode(GLenum drawMode)
{
	m_InstancedRenderer.drawMode = drawMode;
}


