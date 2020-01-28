#pragma once
#include <vector>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Clock.h"
#include "Camera.h"
#include "InputHandler.h"
#include "VAOLoader.h"
#include "shader_setup.h"
#include "Cube.h"

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

	Camera* mainCamera;

	VAOLoader* loader;

	GLuint shader;

	Cube* newCube;

	std::vector<GameObject*> gameObjects;


	//temp
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

};

