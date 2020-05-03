#include "Object3D.h"

Object3D::ModelBuffer Object3D::modelBuffer;

void Object3D::init(VAOData* vaoData, std::vector<GLuint> textureIDs, GLuint shader)
{
	m_VaoData = vaoData;

	setShaderProgram(shader);

	if (shader == ShaderManager::getShader("terrain_tess_basic")) enableTesselation(shader);
	else disableTesselation(shader);
	

	m_TextureIDs = textureIDs;

	//Initialize texture locations

	//Initialize texture locations
	GLuint s1 = ShaderManager::getShader("terrain_basic");
	GLuint s2 = ShaderManager::getShader("terrain_tess_basic");
	glUseProgram(s1);
	glUniform1i(glGetUniformLocation(s1, "texSampler0"), 0);
	glUniform1i(glGetUniformLocation(s1, "texSampler1"), 1);
	glUniform1i(glGetUniformLocation(s1, "texSampler2"), 2);
	glUniform1i(glGetUniformLocation(s1, "texSampler3"), 3);
	glUniform1i(glGetUniformLocation(s1, "texSampler4"), 4);
	glUseProgram(s2);
	glUniform1i(glGetUniformLocation(s2, "texSampler0"), 0);
	glUniform1i(glGetUniformLocation(s2, "texSampler1"), 1);
	glUniform1i(glGetUniformLocation(s2, "texSampler2"), 2);
	glUniform1i(glGetUniformLocation(s2, "texSampler3"), 3);
	glUniform1i(glGetUniformLocation(s2, "texSampler4"), 4);



	//initialize with identity matrix
	//transform = glm::mat4(1.0f);
	initModelUBO();

	onInit();
}

void Object3D::initModelUBO()
{
	//iterate through all shader programs
	for (auto shader : ShaderManager::shaderPrograms) {

		//Escape function if a shader has been set that does not have a model ubo
		if (glGetUniformBlockIndex(shader.second, "Model") != GL_INVALID_INDEX) {
			unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.second, "Model");
			glUniformBlockBinding(shader.second, uniformBlockIndex, 1);

			//Setup UBO
			glGenBuffers(1, &modelUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
			glBindBufferRange(GL_UNIFORM_BUFFER, 1, modelUBO, 0, sizeof(modelBuffer));

		}
	}

	updateModelUBO();
}

void Object3D::onInit()
{
}

Object3D::Object3D()
{
}

Object3D::~Object3D()
{
	delete m_VaoData;
}

void Object3D::update()
{
	//perform derived object specific updates
	onUpdate();

	//Perform required updates for any object3d
	//update UBOs etc..
	updateModelUBO();

}

void Object3D::onUpdate()
{
}

void Object3D::updateModelUBO()
{
	//Fill struct with updated data
	modelBuffer.transform = transform;

	//Fill UBO with struct data
	glBindBuffer(GL_UNIFORM_BUFFER, modelUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(modelBuffer), &modelBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Object3D::draw()
{
	//Bind model UBO
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, modelUBO, 0, sizeof(modelBuffer));


	//Decide whether to use wireframe or not
	if (wireframeEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Set shader program active
	glUseProgram(m_Renderer.shaderProgram);

	//Render
	m_Renderer.render(m_VaoData, m_TextureIDs);
}

void Object3D::enableTesselation(GLuint shader)
{
	//Set tesselation shader program
	setShaderProgram(shader);
	//Set draw mode
	setDrawMode(GL_PATCHES);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	

}

void Object3D::disableTesselation(GLuint shader)
{
	setShaderProgram(shader);
	//Back to default draw
	setDrawMode(GL_TRIANGLES);


}

void Object3D::setWireFrame(bool state)
{
	wireframeEnabled = state;
}

void Object3D::setShaderProgram(GLuint shader)
{
	m_Renderer.shaderProgram = shader;
}

void Object3D::setDrawMode(GLenum drawMode)
{
	m_Renderer.drawMode = drawMode;
}


