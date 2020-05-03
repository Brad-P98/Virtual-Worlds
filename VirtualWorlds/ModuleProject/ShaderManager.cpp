#include "ShaderManager.h"

std::map<std::string, GLuint> ShaderManager::shaderPrograms;


void ShaderManager::addShader(const std::string & shaderName, GLuint ID)
{
	shaderPrograms.insert(std::make_pair(shaderName, ID));
}

const GLuint ShaderManager::getShader(const std::string & shaderName)
{
	return shaderPrograms.at(shaderName);
}

void ShaderManager::initTextureLocations()
{
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
}

void ShaderManager::cleanup()
{
	std::map<std::string, GLuint>::iterator i;
	for (i = shaderPrograms.begin(); i != shaderPrograms.end(); ++i)
	{
		GLuint pr = i->second;
		glDeleteProgram(pr);
	}
	shaderPrograms.clear();
}
