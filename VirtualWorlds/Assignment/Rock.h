#pragma once
#include <Object3D.h>
#include <InstancedObject3D.h>

class Rock : public InstancedObject3D
{

public:
	Rock(glm::vec3 pos, float expiryDist);
	~Rock();

	void onInit() override;

	void onUpdate() override;

private:
	glm::mat4 createRandomModelMatrix(glm::vec3 worldPos);
};

