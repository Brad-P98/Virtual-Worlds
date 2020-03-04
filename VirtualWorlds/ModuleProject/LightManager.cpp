#include "LightManager.h"

std::vector<DirectionalLight*> LightManager::dirLights;
const int LightManager::MAX_LIGHTS = 20;
unsigned int LightManager::dirLightsUBO;
LightManager::DirLightingBuffer LightManager::dirLightingBuffer;


void LightManager::initLightsUBOs()
{
	for (auto shader : ShaderManager::shaderPrograms) {

		//If this shader does have a "Lights" UBO
		if (glGetUniformBlockIndex(shader.second, "Lights") != GL_INVALID_INDEX) {
			unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.second, "Lights");
			glUniformBlockBinding(shader.second, uniformBlockIndex, 2);

			glGenBuffers(1, &dirLightsUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, dirLightsUBO);
			glBindBufferRange(GL_UNIFORM_BUFFER, 2, dirLightsUBO, 0, sizeof(dirLightingBuffer));
		}
	}
	updateLightsUBOs();
}


void LightManager::updateLightsUBOs()
{
	//First update lights buffer with lights from vector
	for (int i = 0; i < dirLights.size(); i++)
	{
		dirLightingBuffer.lights[i] = *dirLights[i];
	}
	dirLightingBuffer.numLights = dirLights.size();

	//Fill ubo with lighting buffer data
	glBindBuffer(GL_UNIFORM_BUFFER, dirLightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(dirLightingBuffer), &dirLightingBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void LightManager::addLight(DirectionalLight * light)
{
	if (dirLights.size() < 20) {
		dirLights.push_back(light);
		updateLightsUBOs();
	}
	else std::cout << "Max lights already instantiated" << std::endl;

}

void LightManager::removeLight(DirectionalLight * light)
{
	for (int i = 0; i < dirLights.size(); i++) {
		if (dirLights[i] = light) {
			dirLights.erase(dirLights.begin() + i);
			delete light;
			updateLightsUBOs();

			break;
		}
	}
}


