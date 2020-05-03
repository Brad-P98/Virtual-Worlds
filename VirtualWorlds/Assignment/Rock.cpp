#include "Rock.h"
#include <VAOLoader.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Instance.h>



Rock::Rock(glm::vec3 pos)
{

	transform = glm::mat4(1);
	////TRANSLATE
	transform = glm::translate(transform, pos);

	////ROTATE
	int randRotation = rand() % 361;		//0-360
	float rads = glm::radians((float)randRotation);
	transform = glm::rotate(transform, rads, glm::vec3(0, 1, 0));

	//SCALE
	int randScaleFactor = rand() % 25 + 1;	//1-25 inclusive
	//Scale each dimension by this value
	transform = glm::scale(transform, glm::vec3((float)randScaleFactor / 100));



	//Chance to use texture
	std::vector<GLuint> texIDs;
	int texRand = rand() % 2;
	if(texRand == 0) texIDs.push_back(TextureManager::getTextureID("Assets/gravel_terrain.jpg"));
	else texIDs.push_back(TextureManager::getTextureID("perlin_noise"));

	init(VAOLoader::getModelVAO("rock.obj"), texIDs, ShaderManager::getShader("basic"));
	Instance::m_scene->addObject(this);
}

Rock::~Rock()
{
}
