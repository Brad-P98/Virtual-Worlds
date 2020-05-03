#include "NoiseGenerator.h"


float NoiseGenerator::generateTotalNoise(float xPos, float zPos)
{
	float totalNoise = 0.0f;

	//Add all noise layers together
	for (auto l : layers) {
		totalNoise += l->amplitude * noiseGenerator->noise(xPos, zPos, l->frequency, NULL);
	}

	return totalNoise;
}
