#pragma once
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Scene.h"
#include "InputHandler.h"


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 640;

Scene* scene;

int init(int argc, char** argv);

//GLUT function callbacks
void drawFrame();
void mouseFunction(int button, int state, int x, int y);
void mouseMoveFunction(int x, int y);
void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
