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
#include "Object3D.h"

class Scene
{

public:

	Scene();
	~Scene();

	void initScene();

	void update();

	void render();

	void quit();

public:

	void addObject(Object3D* object);

	Camera* getMainCamera() const
	{
		return mainCamera;
	}

private:

	Camera* mainCamera;

	VAOLoader* loader;

	GLuint shader;

	std::vector<GameObject*> gameObjects;
};

