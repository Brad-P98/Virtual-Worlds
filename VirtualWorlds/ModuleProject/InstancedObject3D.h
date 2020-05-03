#pragma once
#include <iostream>
#include <vector>
#include <glm/glm/gtc/type_ptr.hpp>


#include "Clock.h"
#include "ShaderManager.h"
#include "GameObject.h"

struct StaticInstance
{

	glm::vec3 worldPos;
	glm::mat4 modelMat;
	float lifetime;
	float expiryDist;
	int id;
};

class InstancedObject3D : public GameObject
{
public:

	InstancedObject3D();
	virtual ~InstancedObject3D();

	virtual void update() override;
	virtual void draw() override;

	void addInstance(StaticInstance* newModel);
	void removeInstance(int id);

	GLuint getShaderProgram() const { return m_InstancedRenderer.shaderProgram; }
	VAOData* getVAOData() const { return m_VaoData; }

protected:

	void init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader);

	void setShaderProgram(GLuint shader);
	void setDrawMode(GLenum drawMode);

	//Default Initialization and update. Called from within child instancedObject3ds.
	virtual void onInit();
	virtual void onUpdate();

private:
	unsigned const int maxInstances = 10000;
	unsigned int instanceID = 0;

	std::vector<StaticInstance*> models;

private:

	InstancedRenderer m_InstancedRenderer;
	VAOData* m_VaoData;

	std::vector<GLuint> m_TextureIDs;
};



