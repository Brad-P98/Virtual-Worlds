#pragma once
#include "Clock.h"
#include "InputHandler.h"

class Scene
{

public:

	Scene();
	~Scene();

	void initScene();

	void update();

	void render();

	void key();
	void keyUp();
private:


};

