#include <Instance.h>
#include <Scene.h>
#include <Camera.h>

#include "Terrain.h"
#include "TerrainBehaviour.h"


Instance* instance;
Scene* scene;

Camera* mainCamera;

GLuint mainShader;
GLuint wireframeShader;

const int TERRAIN_CHUNK_RESOLUTION = 10;

int main(int argc, char** argv) {


	scene = new Scene();

	instance = new Instance();
	instance->init(argc, argv, 800, 600, "window", scene);

	Clock::start();
	
	mainShader = setupShaders("shader.vert", "shader.frag");
	wireframeShader = setupShaders("terrain_wireframe.vert", "terrain_wireframe.geom", "terrain_wireframe.frag");

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	mainCamera = scene->getMainCamera();

	Terrain terrain;
	TerrainBehaviour terrainBehaviour;
	terrainBehaviour.setActiveTerrain(&terrain);
	scene->addBehaviour(&terrainBehaviour);

	TerrainChunk terrainChunk1(0,0, mainShader);
	scene->addObject(&terrainChunk1);

	//TerrainChunk terrainChunk2(glm::vec3(TERRAIN_CHUNK_RESOLUTION, 0, 0), TERRAIN_CHUNK_RESOLUTION, mainShader);
	//scene->addObject(&terrainChunk2);

	glutMainLoop();

	delete instance;
	delete scene;

	return 0;
}