#pragma once
#include <GL/gl3w.h>
#include <map>
#include <string>

class ShaderManager
{
public:
	static void addShader(const std::string& name, GLuint ID);

	static const GLuint getShader(const std::string &name);

	static const int getShaderCount()
	{
		return shaderPrograms.size();
	}

	static void initTextureLocations();

	static void cleanup();

	static std::map<std::string, GLuint> shaderPrograms;
private:


};

