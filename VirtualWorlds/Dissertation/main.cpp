#include <Instance.h>
#include <Scene.h>
#include <ShaderManager.h>
#include <Camera.h>

#include "Terrain.h"
#include "TerrainBehaviour.h"


Instance* instance;
Scene* scene;

Camera* mainCamera;


const int TERRAIN_CHUNK_RESOLUTION = 10;

int main(int argc, char** argv) {

	scene = new Scene();

	instance = new Instance();
	instance->init(argc, argv, 1200, 900, "window", scene);

	Clock::start();
	
	GLuint mainShader = setupShaders("shader.vert", "shader.frag");
	GLuint terrain_wireframeShader = setupShaders("terrain_wireframe.vert", "terrain_wireframe.geom", "terrain_wireframe.frag");

	ShaderManager::addShader("basic", mainShader);
	ShaderManager::addShader("terrain_wireframe", terrain_wireframeShader);

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	mainCamera = scene->getMainCamera();
	mainCamera->setWorldPos(glm::vec3(50, 1, 50));

	//Create a new terrain
	Terrain* terrain = new Terrain();
	terrain->shader = terrain_wireframeShader;

	TerrainBehaviour terrainBehaviour;
	terrainBehaviour.setActiveTerrain(terrain);
	scene->addBehaviour(&terrainBehaviour);

	glutMainLoop();

	delete terrain;
	delete instance;
	delete scene;

	return 0;
}