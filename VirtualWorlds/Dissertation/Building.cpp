#include "Building.h"

#include <VAOLoader.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Instance.h>

Building::Building(glm::vec3 pos, float heightModifier)
{
	transform = glm::translate(glm::mat4(1), pos);

	transform = glm::scale(transform, glm::vec3(0.2f, 0.2f * heightModifier, 0.2f));

	std::vector<GLuint> texIDs;
	texIDs.push_back(TextureManager::getTextureID("Assets/default.jpg"));

	init(VAOLoader::getModelVAO("box.obj"), texIDs, ShaderManager::getShader("basic"));
	Instance::m_scene->addObject(this);
}

Building::~Building()
{
}

