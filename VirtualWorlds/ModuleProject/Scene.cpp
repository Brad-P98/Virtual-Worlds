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

	//update all objects
	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->update();
	}

	InputHandler::lateUpdate();
}

void Scene::render()
{
	//update all objects
	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->draw();
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

void Scene::setSkybox(Skybox * s)
{
	skybox = s;
}
