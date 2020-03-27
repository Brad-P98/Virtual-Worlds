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
class NoiseGenerator {

public:

	std::vector<NoiseLayer*> layers;

	PerlinNoise* noiseGenerator;

	float generateTotalNoise(float xPos, float zPos);
};

