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
	
	newCube = new Cube(loader->loadToVAO(vertices));
	newCube->setShaderProgram(shader);

}

void Scene::update()
{
	Clock::tick();
	InputHandler::update();


	mainCamera->update();

	//update all objects
	newCube->update();

	glm::mat4 modelTrans = glm::mat4(1.0f);

	modelTrans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5)) * 
		glm::rotate(glm::mat4(1.0f), (float)glm::radians(3 * Clock::currTime / 10), glm::vec3(1.0, 0.0, 0.0));

	unsigned int transformLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelTrans));





	/*unsigned int viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(mainCamera->getViewMatrix()));

	unsigned int projLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(mainCamera->getProjMatrix()));*/


}

void Scene::render()
{
	newCube->draw();
}

void Scene::quit()
{
	glutLeaveMainLoop();
}
