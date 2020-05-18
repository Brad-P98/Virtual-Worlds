#include "InstancedObject3D.h"
#include "VAOLoader.h"

#include "Instance.h"


InstancedObject3D::InstancedObject3D()
{
}

InstancedObject3D::~InstancedObject3D()
{
	delete m_VaoData;
}



void InstancedObject3D::init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader)
{

	m_VaoData = vaoData;

	VBO = VAOLoader::getInstance()->createEmptyVBO(instanceDataLength * maxInstances);

	vboContents.resize(instanceDataLength * maxInstances);

	initializeInstanceAttributes();

	setShaderProgram(shader);

	m_TextureIDs = textureIDs;

	onInit();
}

void InstancedObject3D::initializeInstanceAttributes()
{	
	//Mat4
	VAOLoader::addInstanceAttribute(m_VaoData->getVaoID(), VBO, 5, 4, instanceDataLength, 0);
	VAOLoader::addInstanceAttribute(m_VaoData->getVaoID(), VBO, 6, 4, instanceDataLength, 4);
	VAOLoader::addInstanceAttribute(m_VaoData->getVaoID(), VBO, 7, 4, instanceDataLength, 8);
	VAOLoader::addInstanceAttribute(m_VaoData->getVaoID(), VBO, 8, 4, instanceDataLength, 12);
}

void InstancedObject3D::addInstance(StaticInstance* newModel)
{
	if (modelMap.size() >= maxInstances) return;
	newModel->id = instanceID;
	instanceID++;
	modelMap.insert(std::make_pair(newModel->id, newModel));

	rebuildFlag = true;
}

void InstancedObject3D::removeInstance(StaticInstance* inst)
{
	auto itt = modelMap.find(inst->id);
	if (itt != modelMap.end()) {
		//Remove model
		modelMap.erase(itt);
		//Removed, so need to rebuild
		rebuildFlag = true;
	}

}

void InstancedObject3D::rebuildVBOContents()
{
	std::map<int, StaticInstance*> tempMap = modelMap;
	int ptr = 0;
	auto itt = tempMap.begin();
	for (itt = tempMap.begin(); itt != tempMap.end(); itt++) {
		const float *pSource = (const float*)glm::value_ptr(itt->second->modelMat);
		//Push into VBO contents
		for (int i = 0; i < 16; ++i) {
			vboContents[ptr++] = pSource[i];
		}
	}
	VBORebuilt = true;
	
}


void InstancedObject3D::update()
{
	//Update VBO contents
	if (rebuildFlag && !building) {

		thread = std::thread(&InstancedObject3D::rebuildVBOContents, this);
		building = true;
		//VAOLoader::getInstance()->updateInstanceVBO(VBO, vboContents);
		rebuildFlag = false;
	}
	if (VBORebuilt) {
		VAOLoader::getInstance()->updateInstanceVBO(VBO, vboContents);
		VBORebuilt = false;
		building = false;
		thread.join();
	}

	onUpdate();

}

void InstancedObject3D::draw()
{

	//Use shader program
	glUseProgram(m_InstancedRenderer.shaderProgram);

	m_InstancedRenderer.render(m_VaoData, modelMap.size(), m_TextureIDs);
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


