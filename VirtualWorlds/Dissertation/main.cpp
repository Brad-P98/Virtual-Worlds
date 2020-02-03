#include <Wrapper.h>
#include <Scene.h>

#include "Box.h"

#include "Terrain.h"

Wrapper* instance;
Scene* scene;

GLuint mainShader;

//temp box vertices
std::vector<float> vertices = {
-0.6f,-0.6f,-0.6f, // triangle 1 : begin
-0.6f,-0.6f, 0.6f,
-0.6f, 0.6f, 0.6f, // triangle 1 : end
0.6f, 0.6f,-0.6f, // triangle 2 : begin
-0.6f,-0.6f,-0.6f,
-0.6f, 0.6f,-0.6f, // triangle 2 : end
0.6f,-0.6f, 0.6f,
-0.6f,-0.6f,-0.6f,
0.6f,-0.6f,-0.6f,
0.6f, 0.6f,-0.6f,
0.6f,-0.6f,-0.6f,
-0.6f,-0.6f,-0.6f,
-0.6f,-0.6f,-0.6f,
-0.6f, 0.6f, 0.6f,
-0.6f, 0.6f,-0.6f,
0.6f,-0.6f, 0.6f,
-0.6f,-0.6f, 0.6f,
-0.6f,-0.6f,-0.6f,
-0.6f, 0.6f, 0.6f,
-0.6f,-0.6f, 0.6f,
0.6f,-0.6f, 0.6f,
0.6f, 0.6f, 0.6f,
0.6f,-0.6f,-0.6f,
0.6f, 0.6f,-0.6f,
0.6f,-0.6f,-0.6f,
0.6f, 0.6f, 0.6f,
0.6f,-0.6f, 0.6f,
0.6f, 0.6f, 0.6f,
0.6f, 0.6f,-0.6f,
-0.6f, 0.6f,-0.6f,
0.6f, 0.6f, 0.6f,
-0.6f, 0.6f,-0.6f,
-0.6f, 0.6f, 0.6f,
0.6f, 0.6f, 0.6f,
-0.6f, 0.6f, 0.6f,
0.6f,-0.6f, 0.6f
};

const int TERRAIN_CHUNK_RESOLUTION = 10;

int main(int argc, char** argv) {


	scene = new Scene();

	instance = new Wrapper();
	instance->init(argc, argv, 800, 600, "window", scene);

	Clock::start();
	
	mainShader = setupShaders("shader.vert", "shader.frag");

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	//Box* box = new Box(loader->loadToVAO(vertices), mainShader);
	//scene->addObject(box);

	TerrainChunk terrainChunk1(0,0, mainShader);
	scene->addObject(&terrainChunk1);

	//TerrainChunk terrainChunk2(glm::vec3(TERRAIN_CHUNK_RESOLUTION, 0, 0), TERRAIN_CHUNK_RESOLUTION, mainShader);
	//scene->addObject(&terrainChunk2);

	glutMainLoop();

	return 0;
}