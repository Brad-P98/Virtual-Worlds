#include "Building.h"

#include <VAOLoader.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Instance.h>

Building::Building(glm::vec3 pos, float heightModifier)
{
	transform = glm::translate(glm::mat4(1), pos);

	transform = glm::scale(transform, glm::vec3(0.15f, 0.15f * heightModifier, 0.15f));


	thisInstance = new StaticInstance();
	//If instance of this object exists, add instance.
	if (Instance::m_scene->instancedObjectExists("Building")) {

		thisInstance->modelMat = transform;
		Instance::m_scene->getInstancedObject("Building")->addInstance(thisInstance);
	}
	else {
		//Create new object to contain instances
		std::vector<GLuint> texIDs;
		texIDs.push_back(TextureManager::getTextureID("Assets/default.jpg"));

		//Create new instanced object, and give it Building's properties.
		InstancedObject3D* newInstancedObject = new InstancedObject3D();
		newInstancedObject->init(VAOLoader::getModelVAO("box.obj"), texIDs, ShaderManager::getShader("instanced_basic"));

		Instance::m_scene->addInstancedObject("Building", newInstancedObject);

		//Create and add first static instance
		thisInstance->modelMat = transform;
		Instance::m_scene->getInstancedObject("Building")->addInstance(thisInstance);
	}

}

Building::~Building()
{
	Instance::m_scene->getInstancedObject("Building")->removeInstance(thisInstance);
}


