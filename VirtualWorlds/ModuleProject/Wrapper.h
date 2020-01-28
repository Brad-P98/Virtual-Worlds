#pragma once
#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>

#include <iostream>

#include "Scene.h"
#include "InputHandler.h"

void drawFrame();
class Wrapper;
static Wrapper * wrapperInstance = nullptr;
	
class Wrapper
{
public:
	Wrapper();
	~Wrapper();

	Scene* m_scene;

	int init(int argc, char** argv, int winWidth, int winHeight, const char* winName, Scene* scene);


};

