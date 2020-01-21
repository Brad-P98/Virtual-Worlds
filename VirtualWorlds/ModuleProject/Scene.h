#pragma once
#include "Clock.h"
#include "InputHandler.h"
#include "VAOLoader.h"

class Renderer;

class Scene
{

public:

	Scene();
	~Scene();

	void initScene();

	void update();

	void render();

	void quit();

private:

	VAOLoader* loader;
	Renderer* renderer;

};

