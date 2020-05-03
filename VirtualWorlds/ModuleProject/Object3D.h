#pragma once
#include <iostream>
#include <vector>
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

	GLuint getShaderProgram() const { return m_Renderer.shaderProgram; }
	VAOData* getVAOData() const { return m_VaoData; }

	void enableTesselation(GLuint shader);
	void disableTesselation(GLuint shader);
	void setWireFrame(bool state);

protected:


	void init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader);

	void setShaderProgram(GLuint shader);
	void setDrawMode(GLenum drawMode);

	//Default Initialization and update. Only called from within object3d.
	virtual void onInit();
	virtual void onUpdate();

private:

	void initModelUBO();
	void updateModelUBO();

public:

	glm::mat4 transform = glm::mat4(1);

private:

	Renderer m_Renderer;
	VAOData* m_VaoData;

	std::vector<GLuint> m_TextureIDs;

	static struct ModelBuffer {
		glm::mat4 transform;

	} modelBuffer;

	unsigned int modelUBO;

	bool wireframeEnabled;
};

