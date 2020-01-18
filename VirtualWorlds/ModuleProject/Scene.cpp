#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::initScene()
{
	loader = new VAOLoader();
	renderer = new Renderer();
}

void Scene::update()
{
	Clock::tick();

	InputHandler::handleEvents();



}

void Scene::render()
{

}
