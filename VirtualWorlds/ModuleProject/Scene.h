#pragma once
#include "Clock.h"
#include "InputHandler.h"
#include "VAOLoader.h"
#include "VAOData.h"
#include "shader_setup.h"

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

	GLuint shader;
	VAOData* d;


	float vertices[9] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

};

