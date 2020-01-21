#include "Scene.h"
#include "Renderer.h"

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

	shader = setupShaders("shader.vert", "shader.frag");

	
	d = loader->loadToVAO(vertices);
	renderer->shaderProgram = shader;
	glUseProgram(shader);
}

void Scene::update()
{
	Clock::tick();

	InputHandler::handleEvents();

}

void Scene::render()
{
	renderer->render(*d);
}
