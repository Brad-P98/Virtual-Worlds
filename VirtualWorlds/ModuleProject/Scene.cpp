#include "Scene.h"
#include "TextureManager.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initScene()
{

	mainCamera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	mainCamera->initCameraUBOs();

	LightManager::initLightsUBOs();

	TextureManager::loadTexture("Assets/default.jpg");
}

void Scene::update()
{
	Clock::tick();
	InputHandler::update();

	mainCamera->update();

	//update all behaviours
	for (size_t i = 0; i < behaviours.size(); i++) {
		behaviours[i]->update();
	}


	//update all instanced objects.
	for (auto itt = instancedGameObjects.begin(); itt != instancedGameObjects.end(); itt++) {
		itt->second->update();
	}

	//update all objects
	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->update();
	}

	InputHandler::lateUpdate();
}

void Scene::render()
{
	//draw all objects
	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->draw();
	}

	//draw all instanced objects.
	for (auto itt = instancedGameObjects.begin(); itt != instancedGameObjects.end(); itt++) {
		itt->second->draw();
	}

	if (skybox != nullptr) {
		skybox->draw();
	}
}

void Scene::quit()
{
	delete mainCamera;
	glutLeaveMainLoop();
}

void Scene::addObject(Object3D* object)
{
	gameObjects.push_back(object);
}

void Scene::addInstancedObject(std::string objectName, InstancedObject3D * object)
{
	instancedGameObjects.insert(std::make_pair(objectName, object));
}

void Scene::addBehaviour(Behaviour* behaviour)
{
	behaviours.push_back(behaviour);
}

void Scene::removeObject(Object3D* object)
{
	for (int i = 0; i < gameObjects.size(); i++) {
		if (gameObjects[i] == object) {
			gameObjects.erase(gameObjects.begin() + i);

			delete object;

			break;
		}
	}
}

void Scene::removeInstancedObject(std::string objectName)
{
	instancedGameObjects.erase(objectName);
}

void Scene::removeBehaviour(Behaviour* behaviour)
{
	for (int i = 0; i < behaviours.size(); i++) 
	{
		if (behaviours[i] == behaviour) {
			behaviours.erase(behaviours.begin() + i);

			delete behaviour;

			break;
		}
	}

}

bool Scene::instancedObjectExists(std::string objectName)
{
	//Loop through objects
	for (auto itt = instancedGameObjects.begin(); itt != instancedGameObjects.end(); itt++) {
		//Check if object name matches one that exists already
		if (itt->first == objectName) return true;
	}

	return false;
}

InstancedObject3D * Scene::getInstancedObject(std::string objectName)
{
	//Loop through objects
	for (auto itt = instancedGameObjects.begin(); itt != instancedGameObjects.end(); itt++) {
		//Check if object name matches one that exists already
		if (itt->first == objectName) return itt->second;
	}
	return nullptr;
}

void Scene::setSkybox(Skybox * s)
{
	skybox = s;
}
