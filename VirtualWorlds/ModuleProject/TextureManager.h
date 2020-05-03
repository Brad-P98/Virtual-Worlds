#pragma once
#include <GL/gl3w.h>
#include <map>
#include <string>

#include <iostream>

class TextureManager
{
public:

	static void loadTexture(const char* fileName);

	static void addTexture(std::string name, GLuint id);

	static const GLuint getTextureID(const std::string& name);

	static const int getTextureCount() { return textures.size(); }

	static void cleanup();

	static std::map<std::string, GLuint> textures;
};

