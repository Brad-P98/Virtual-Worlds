#include <Wrapper.h>
#include <Scene.h>

#include "Box.h"

#include "Terrain.h"

Wrapper* instance;
Scene* scene;
VAOLoader* loader;

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

int main(int argc, char** argv) {


	scene = new Scene();
	loader = new VAOLoader();

	instance = new Wrapper();
	instance->init(argc, argv, 800, 600, "window", scene);

	Clock::start();
	
	mainShader = setupShaders("shader.vert", "shader.frag");

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	//Box* box = new Box(loader->loadToVAO(vertices), mainShader);
	//scene->addObject(box);

	Terrain* terrain = new Terrain(10, 10, mainShader);
	scene->addObject(terrain);


	glutMainLoop();

	return 0;
}