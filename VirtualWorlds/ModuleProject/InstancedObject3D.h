#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <glm/glm/gtc/type_ptr.hpp>


#include "Clock.h"
#include "ShaderManager.h"
#include "GameObject.h"

struct StaticInstance
{
	glm::mat4 modelMat;
	int expiryDist =10000;
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
	void removeInstance(StaticInstance* instance);

	GLuint getShaderProgram() const { return m_InstancedRenderer.shaderProgram; }
	VAOData* getVAOData() const { return m_VaoData; }

	void init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader);
protected:



	void setShaderProgram(GLuint shader);
	void setDrawMode(GLenum drawMode);

	//Default Initialization and update. Called from within child instancedObject3ds.
	virtual void onInit();
	virtual void onUpdate();

private:

	void initializeInstanceAttributes();

	void rebuildVBOContents();

private:
	unsigned const int maxInstances = 90000;
	unsigned int instanceID = 0;

	GLuint VBO;
	//Number of floats of each instance in the VBO
	int instanceDataLength = 16;

	std::map<int, StaticInstance*> modelMap;

	std::vector<float> vboContents;


private:

	InstancedRenderer m_InstancedRenderer;
	VAOData* m_VaoData;

	std::vector<GLuint> m_TextureIDs;

	bool rebuildFlag = false;
	bool VBORebuilt = false;
	bool building = false;
	std::thread thread;
};



