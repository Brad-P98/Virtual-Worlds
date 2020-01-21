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
	mainCamera = new Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, -1.0));

	loader = new VAOLoader();
	renderer = new Renderer();

	shader = setupShaders("shader.vert", "shader.frag");

	
	d = loader->loadToVAO(vertices);
	renderer->shaderProgram = shader;
	glUseProgram(shader);

	projection = glm::perspective(glm::radians(60.0f), 800.0f/640.0f, 0.1f, 100.0f);

}

void Scene::update()
{

	Clock::tick();
	InputHandler::handleEvents();

	glm::mat4 modelTrans = glm::mat4(1.0f);
	modelTrans = glm::translate(modelTrans, glm::vec3(0.0, 0.0, -5));
	modelTrans = glm::rotate(modelTrans, (float)glm::radians(3 * Clock::currTime/10), glm::vec3(1.0, 1.0, 1.0));

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, mainCamera->position);

	unsigned int transformLoc = glGetUniformLocation(shader, "modelTrans");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelTrans));

	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


}

void Scene::render()
{
	renderer->render(*d);
}
