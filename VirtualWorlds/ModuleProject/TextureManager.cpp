#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::map<std::string, GLuint> TextureManager::textures;

void TextureManager::loadTexture(const char* fileName)
{

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -2);

	int width, height, numChannels;
	unsigned char* data = stbi_load(fileName, &width, &height, &numChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::string str(fileName);

		//add to map
		textures.insert(std::make_pair(str, textureID));
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

}

const GLuint TextureManager::getTextureID(const std::string & name)
{
	for (auto tex : textures) {
		if (tex.first == name) {
			//texture found with matching name
			return tex.second;
		}
	}

	//no texture found with matching name. use default.
	return textures.at("Assets/default.jpg");
}

void TextureManager::cleanup()
{
	//delete all texture buffers
	for (auto texture : textures) {
		glDeleteBuffers(1, &texture.second);
	}
	textures.clear();
}
