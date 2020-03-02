#include "LightManager.h"

std::vector<DirectionalLight*> LightManager::lights;
const int LightManager::MAX_LIGHTS = 20;
unsigned int LightManager::lightsUBO;


void LightManager::initLightsUBO()
{
}


void LightManager::addLight(DirectionalLight * light)
{
	if (lights.size() < 20) {
		lights.push_back(light);
	}
	else std::cout << "Max lights already instantiated" << std::endl;

}

void LightManager::removeLight(DirectionalLight * light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i] = light) {
			lights.erase(lights.begin() + i);
			delete light;
			break;
		}
	}
}


