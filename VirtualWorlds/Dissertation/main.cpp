#include <Wrapper.h>
#include <Scene.h>
#include <Clock.h>


Wrapper* instance;
Scene* scene;

int main(int argc, char** argv) {


	scene = new Scene();

	instance = new Wrapper();
	instance->init(argc, argv, 800, 600, "window", scene);

	Clock::start();

	//Initialises a completely empty scene
	scene->initScene();

	glutMainLoop();

	return 0;
}