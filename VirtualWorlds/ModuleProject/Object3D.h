#pragma once
#include "GameObject.h"


class Object3D : public GameObject
{

public:

	Object3D();
	~Object3D();


	//Default Initialization and update
	virtual void onInit();
	virtual void update() override;
	virtual void draw() override;


	void setShaderProgram(GLuint shader);

protected:

	void init(VAOData* vaoData);

protected:

	Renderer* m_Renderer;
	VAOData* m_VaoData;

	glm::mat4 transform;
};

