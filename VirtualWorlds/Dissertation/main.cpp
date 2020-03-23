#include <Instance.h>
#include <Scene.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Camera.h>
#include <LightManager.h>
#include <Skybox.h>

#include "Terrain.h"
#include "TerrainBehaviour.h"

#include "WaterPlane.h"
#include "WaterPlaneBehaviour.h"



Instance* instance;
Scene* scene;

Camera* mainCamera;



int main(int argc, char** argv) {

	scene = new Scene();

	instance = new Instance();
	instance->init(argc, argv, 1200, 900, "window", scene);

	Clock::start();
	
	GLuint mainShader = setupShaders("shader.vert", "shader.frag");
	GLuint terrain_wireframeShader = setupShaders("terrain_wireframe.vert", "terrain_wireframe.geom", "terrain_wireframe.frag");

	GLuint skyboxShader = setupShaders("skyboxShader.vert", "skyboxShader.frag");

	ShaderManager::addShader("basic", mainShader);
	ShaderManager::addShader("terrain_wireframe", terrain_wireframeShader);
	ShaderManager::addShader("skyboxShader", skyboxShader);


	TextureManager::loadTexture("Assets/grass_terrain.jpg");
	TextureManager::loadTexture("Assets/water.png");

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	mainCamera = scene->getMainCamera();
	mainCamera->setWorldPos(glm::vec3(50, 1, 50));

	Skybox* newSkybox = new Skybox("Skybox", skyboxShader);
	scene->setSkybox(newSkybox);

	//Create an ambient light for the scene
	DirectionalLight* mainLight = new DirectionalLight();
	mainLight->direction = glm::vec4(0.6f, 1.0f, 0.3f, 0.0f);
	mainLight->ambient = 0.02f;
	mainLight->colourIntensities = glm::vec4(0.7f, 0.7f, 0.7f, 0.0f);
	LightManager::addLight(mainLight);

	//Create a new terrain
	Terrain* terrain = new Terrain();
	terrain->shader = mainShader;

	TerrainBehaviour terrainBehaviour;
	terrainBehaviour.setActiveTerrain(terrain);
	scene->addBehaviour(&terrainBehaviour);


	WaterPlane* water = new WaterPlane();
	water->shader = mainShader;

	WaterPlaneBehaviour waterBehaviour;
	waterBehaviour.setActiveWaterPlane(water);
	scene->addBehaviour(&waterBehaviour);

	glutMainLoop();

	ShaderManager::cleanup();
	TextureManager::cleanup();
	delete terrain;
	delete instance;
	delete scene;

	return 0;
}