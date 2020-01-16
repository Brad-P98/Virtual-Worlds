#pragma once
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <glfw-3.3/glfw-3.3/include/GLFW/glfw3.h>
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
void mouseMove(int x, int y);
void mouseDrag(int x, int y);
void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
