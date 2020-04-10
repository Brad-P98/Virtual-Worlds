#pragma once
#include <iostream>
#include <glm/glm/gtc/type_ptr.hpp>

#include "Clock.h"
#include "GameObject.h"
#include "ShaderManager.h"


class Object3D : public GameObject
{

public:

	Object3D();
	virtual ~Object3D();


	virtual void update() override;
	virtual void draw() override;

	GLuint getShaderProgram() { return m_Renderer.shaderProgram; }

protected:


	void init(VAOData* vaoData, GLuint textureID, GLuint shader);
	void setShaderProgram(GLuint shader);

	//Default Initialization and update. Only called from within object3d.
	virtual void onInit();
	virtual void onUpdate();

private:

	void initModelUBO();
	void updateModelUBO();

public:

	glm::mat4 transform;

private:

	Renderer m_Renderer;
	VAOData* m_VaoData;

	GLuint m_TextureID;

	static struct ModelBuffer {
		glm::mat4 transform;

	} modelBuffer;

	unsigned int modelUBO;
};

