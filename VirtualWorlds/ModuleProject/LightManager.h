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

	static struct DirLightingBuffer
	{
		DirectionalLight lights[20];
		int numLights;
	} dirLightingBuffer;

	static unsigned int dirLightsUBO;

	static std::vector<DirectionalLight*> dirLights;

	static const int MAX_LIGHTS;


};

