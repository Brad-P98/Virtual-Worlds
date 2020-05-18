#pragma once
#include <InstancedObject3D.h>
#include <Object3D.h>

class Building
{
public:

	Building(glm::vec3 pos, float heightModifier);
	~Building();

private:

	glm::mat4 transform;
	StaticInstance* thisInstance;

};

