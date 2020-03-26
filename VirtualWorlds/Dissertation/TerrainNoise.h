#pragma once
#include <vector>
#include <PerlinNoise.hpp>

//Single noise layer
struct NoiseLayer {
	int amplitude;
	float frequency;

	NoiseLayer(int a, float f) : amplitude(a), frequency(f) {};
};

//Easily accessible struct containing all noise layers actively being used.
static class TerrainNoise {

public:

	static std::vector<NoiseLayer*> layers;

	static PerlinNoise* noiseGenerator;

	static float generateTotalNoise(float xPos, float zPos);
};

