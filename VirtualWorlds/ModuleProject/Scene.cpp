#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initScene()
{

}

void Scene::update()
{
	Clock::tick();

	std::cout << InputHandler::mouseEventQueue.size();
	InputHandler::Input::handleEvents();



}

void Scene::render()
{

}
