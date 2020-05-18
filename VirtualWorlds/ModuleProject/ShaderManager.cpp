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
	GLuint s3 = ShaderManager::getShader("basic");
	GLuint s4 = ShaderManager::getShader("water_basic");
	GLuint s5 = ShaderManager::getShader("instanced_basic");
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
	glUseProgram(s3);
	glUniform1i(glGetUniformLocation(s3, "texSampler0"), 0);
	glUniform1i(glGetUniformLocation(s3, "texSampler1"), 1);
	glUniform1i(glGetUniformLocation(s3, "texSampler2"), 2);
	glUniform1i(glGetUniformLocation(s3, "texSampler3"), 3);
	glUniform1i(glGetUniformLocation(s3, "texSampler4"), 4);
	glUseProgram(s4);
	glUniform1i(glGetUniformLocation(s4, "texSampler0"), 0);
	glUniform1i(glGetUniformLocation(s4, "texSampler1"), 1);
	glUniform1i(glGetUniformLocation(s4, "texSampler2"), 2);
	glUniform1i(glGetUniformLocation(s4, "texSampler3"), 3);
	glUniform1i(glGetUniformLocation(s4, "texSampler4"), 4);
	glUseProgram(s5);
	glUniform1i(glGetUniformLocation(s5, "texSampler0"), 0);
	glUniform1i(glGetUniformLocation(s5, "texSampler1"), 1);
	glUniform1i(glGetUniformLocation(s5, "texSampler2"), 2);
	glUniform1i(glGetUniformLocation(s5, "texSampler3"), 3);
	glUniform1i(glGetUniformLocation(s5, "texSampler4"), 4);
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
