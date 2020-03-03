#include "LightManager.h"

std::vector<DirectionalLight*> LightManager::lights;
const int LightManager::MAX_LIGHTS = 20;
unsigned int LightManager::lightsUBO;
LightManager::LightingBuffer LightManager::lightingBuffer;


void LightManager::initLightsUBOs()
{
	for (auto shader : ShaderManager::shaderPrograms) {

		//If this shader does have a "Lights" UBO
		if (glGetUniformBlockIndex(shader.second, "Lights") != GL_INVALID_INDEX) {
			unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.second, "Lights");
			glUniformBlockBinding(shader.second, uniformBlockIndex, 2);

			glGenBuffers(1, &lightsUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
			glBindBufferRange(GL_UNIFORM_BUFFER, 2, lightsUBO, 0, sizeof(lightingBuffer));
		}
	}
	updateLightsUBOs();
}


void LightManager::updateLightsUBOs()
{
	//First update lights buffer with lights from vector
	for (int i = 0; i < lights.size(); i++)
	{
		lightingBuffer.lights[i] = *lights[i];
	}
	lightingBuffer.numLights = lights.size();

	//Fill ubo with lighting buffer data
	glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightingBuffer), &lightingBuffer, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void LightManager::addLight(DirectionalLight * light)
{
	if (lights.size() < 20) {
		lights.push_back(light);
		updateLightsUBOs();
	}
	else std::cout << "Max lights already instantiated" << std::endl;

}

void LightManager::removeLight(DirectionalLight * light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i] = light) {
			lights.erase(lights.begin() + i);
			delete light;
			updateLightsUBOs();

			break;
		}
	}
}


