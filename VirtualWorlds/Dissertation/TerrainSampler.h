#pragma once
#include "ChunkSettings.h"
#include "Terrain.h"


//The terrain sampler looks for featuresof the terrain chunk and scores vertices based on their suitability to host a settlement.

//Vertices near sea level are immediately good candidates for settlements.
//Vertices sampled are given a score, which can be added to or subtracted from based on the presence of features in the surrounding area.

//Terrain chunks are sampled individually. Sampling settlement proximity can look outside of the chunk's bounds, however only the vertices within the bounds are scored.

//The vertex with the highest score becomes the 'focal point' of a new settlement.
class TerrainSampler
{
public:

	TerrainSampler();
	~TerrainSampler();

	std::vector<int> sampleChunk(TerrainChunk*);

private:
	//1. SampleGradient
	//The steeper the gradient, the fewer points awarded. Check only immediately surrounding vertices.
	int calcGradientScore(float xPos, float zPos);

	//2. SampleAltitude
	//Any vertices below sea level are immediately 0.
	//Vertices at a particularly high altitude are also less desirable.
	int calcAltitudeScore(float xPos, float zPos);

	//3. SampleSettlementProximity
	//Every settlement has a 'focal point' vertex, as well as a value suggesting the size of the settlement. This iterates through focal points that are potentially nearby. If a focal point is nearby, reduce the score of the vertex, but only if it is below the threshold that decides if buildings should be there.
	int calcSettlementProxScore(float xPos, float zPos);

};

