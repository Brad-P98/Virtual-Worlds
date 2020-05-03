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
#include "InstancedObject3D.h"
#include "Behaviour.h"

#include "LightManager.h"
#include "Skybox.h"


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

	void setSkybox(Skybox* skybox);

	void addObject(Object3D* object);
	void addInstancedObject(std::string objectName, InstancedObject3D* object);
	void addBehaviour(Behaviour* behaviour);

	void removeObject(Object3D* object);
	void removeInstancedObject(std::string objectName);
	void removeBehaviour(Behaviour* behaviour);

	bool instancedObjectExists(std::string objectName);
	InstancedObject3D* getInstancedObject(std::string objectName);

	Camera* getMainCamera() const
	{
		return mainCamera;
	}


private:

	Camera* mainCamera;

	VAOLoader* loader;

	GLuint shader;
	std::vector<GLuint> shaderPrograms;

	std::vector<Object3D*> gameObjects;
	std::map<std::string, InstancedObject3D*> instancedGameObjects;
	std::vector<Behaviour*> behaviours;

	Skybox* skybox;
};

