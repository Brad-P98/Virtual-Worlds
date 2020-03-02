#pragma once
#include <vector>
#include <iostream>

#include "Light.h"

class LightManager
{
public:
	static void addLight(DirectionalLight* light);
	static void removeLight(DirectionalLight* light);

private:

	static void initLightsUBO();
private:

	static std::vector<DirectionalLight*> lights;

	static const int MAX_LIGHTS;

	static unsigned int lightsUBO;
};

