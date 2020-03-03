#pragma once
#include <vector>
#include <iostream>

#include "Light.h"
#include "ShaderManager.h"

class LightManager
{
public:
	static void addLight(DirectionalLight* light);
	static void removeLight(DirectionalLight* light);

	static void initLightsUBOs();

private:

	static void updateLightsUBOs();

private:

	static struct LightingBuffer
	{
		DirectionalLight lights[20];
		int numLights;
	} lightingBuffer;


	static std::vector<DirectionalLight*> lights;

	static const int MAX_LIGHTS;

	static unsigned int lightsUBO;
};

