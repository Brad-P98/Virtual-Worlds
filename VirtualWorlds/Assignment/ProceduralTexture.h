#pragma once
#include <ShaderManager.h>
#include <glm/glm/glm.hpp>

class ProceduralTexture
{
public:

	ProceduralTexture();
	~ProceduralTexture();

	GLuint generateTexture(GLuint w, GLuint h);

	void setComputeShader(GLuint shader) { m_Shader = shader; }

private:

	GLuint m_Shader;

	GLuint perlinModelBuffer;

};

