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

	shader = setupShaders("shader.vert", "shader.frag");

	mainCamera = new Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
	//pass in whatever shader we want to setup the UBO for
	//TODO: be able to setup this same UBO for multiple different shader programs
	mainCamera->initUBOs(shader);
	
	newCube = new Cube(loader->loadToVAO(vertices), shader);

}

void Scene::update()
{
	Clock::tick();
	InputHandler::update();


	mainCamera->update();

	//update all objects
	newCube->update();


}

void Scene::render()
{
	newCube->draw();
}

void Scene::quit()
{
	glutLeaveMainLoop();
}
