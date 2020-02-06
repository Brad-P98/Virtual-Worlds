#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initScene()
{

	mainCamera = new Camera(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	mainCamera->initUBOs();

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

}

void Scene::render()
{
	//update all objects
	for (size_t i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->draw();
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
			break;
		}
	}

}
