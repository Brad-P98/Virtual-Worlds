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
