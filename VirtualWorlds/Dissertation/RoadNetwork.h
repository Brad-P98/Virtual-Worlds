#pragma once
#include <glm/glm/glm.hpp>
#include <vector>
#include <map>
#include "RoadSegment.h"
#include "PathNodes.h"

#include "NoiseGenerator.h"

class Settlement;

namespace std 
{
	bool operator <(const glm::vec3& LHand, const glm::vec3& RHand);
}


//Every settlement builds a road network
class RoadNetwork
{
private:
	static RoadNetwork* instance;

public:

	RoadNetwork(NoiseGenerator* tNoise);
	~RoadNetwork();
	static RoadNetwork* getInstance() { return instance; }
	static void setInstance(RoadNetwork* inst) { instance = inst; }


	void addNode(Settlement* newNode);

	void linkNode(HLPathNode* node);

	void removeNode(Settlement* nodeToDelete);

	HLPathNode* getHLNode(Settlement* settlement);


private:

	std::vector<HLPathNode*> calcHLNodeNeighbours(HLPathNode* node);

	std::vector<LLPathNode*> getPath(glm::vec3 start, glm::vec3 dest);

	//Search map for neighbours
	std::vector<LLPathNode*> getLLNodeNeighbours(LLPathNode* currNode);

	LLPathNode* getLLNode(glm::vec3 pos);

	float calcDist(glm::vec3 startPosition, glm::vec3 endPosition);

private:

	//Container for all roads - Can access which HL nodes they belong to
	std::vector<RoadSegment*> allRoads;

	//Store Pairs of settlement IDs as start and end points of main roads.
	std::vector<std::pair<int, int>> settlementConnections;


	//Store HL nodes and all connections between them
	std::map<int, HLPathNode*> highLevelNodes;

	//Using vec3 as key -- uses custom < operator overload
	//Cleared after each A* pass
	std::map<glm::vec3, LLPathNode*> allNodes;

	NoiseGenerator* terrainNoise;

};

