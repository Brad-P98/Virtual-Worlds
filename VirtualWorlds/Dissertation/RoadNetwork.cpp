#include "RoadNetwork.h"
#include "SettlementManager.h"
#include <list>
#include <map>

#include "ChunkSettings.h"
#include "Building.h"

#define _USE_MATH_DEFINES
#include <math.h>

RoadNetwork* RoadNetwork::instance;


RoadNetwork::RoadNetwork(NoiseGenerator* tNoise)
{
	terrainNoise = tNoise;
}

RoadNetwork::~RoadNetwork()
{
	//Delete all road segments
	for (int i = 0; i < allRoads.size(); i++) {
		delete allRoads[i];
		allRoads.clear();
	}

	//Delete High level nodes
	auto itt = highLevelNodes.begin();
	for (auto h : highLevelNodes) {
		//Remove node
		delete h.second;
	}
	highLevelNodes.clear();
}


//Pretty much just set up the node and work out its neighbours
void RoadNetwork::addNode(Settlement * sett)
{
	//If node exists for this settlement, return. Else, continue to create.
	if (highLevelNodes.find(sett->settlementID) != highLevelNodes.end()) return;

	//Create node
	HLPathNode* newPathNode = new HLPathNode(sett->settlementID, sett->focalPointPos);
	highLevelNodes.insert(std::make_pair(sett->settlementID, newPathNode));	//Add to rest of nodes

	std::vector<HLPathNode*> neighbours = calcHLNodeNeighbours(newPathNode);
	newPathNode->m_Neighbours = neighbours;
	//update the neighbours to include this new node as their neighbour.
	for (int i = 0; i < neighbours.size(); i++) {
		neighbours[i]->m_Neighbours.push_back(newPathNode);
	}

}

void RoadNetwork::linkNode(HLPathNode * node)
{
	//Now try to draw roads to these neighbours
	for (int i = 0; i < node->m_Neighbours.size(); i++) {
		
		allNodes.clear();

		float bestDist = INFINITY;
		int startIndex = 0;
		int endIndex = 0;
		//Work out which connecting nodes to use.
		for (int j = 0; j < node->connectingPoints.size(); j++) {
			for (int k = 0; k < node->m_Neighbours[i]->connectingPoints.size(); k++) {
				float dist = calcDist(node->connectingPoints[j], node->m_Neighbours[i]->m_WorldPos);
				if (dist < bestDist) {
					//New best distance found - remember which positions in the vectors they were.
					bestDist = dist;
					startIndex = j;
					endIndex = k;

				}
			}
		}

		std::vector<LLPathNode*> path = getPath(node->m_WorldPos, node->m_Neighbours[i]->m_WorldPos);
		allNodes.clear();

		if (path.size() == 0) {
			//If no path found, remove parent HLnodes as neighbours of one another
			//std::cout << "no path" << std::endl;
			continue;
		}
		//std::cout << "got Path" << std::endl;

		//Iterate through path, placing road segments with their correct HLNodeIDs
		for (int j = 1; j < path.size() - 2; j++) {
			//Building* b = new Building(path[j]->m_WorldPos, 1);
			RoadSegment* newRoadSeg = new RoadSegment(path[j]->m_WorldPos + glm::vec3(0,0.5,0), path[j + 1]->m_WorldPos + glm::vec3(0, 0.5, 0));
			newRoadSeg->HLNodeIDs = std::make_pair(node->m_ID, node->m_Neighbours[i]->m_ID);
			allRoads.push_back(newRoadSeg);
		}

	}


}

void RoadNetwork::removeNode(Settlement * nodeToDelete)
{
	auto itt = highLevelNodes.find(nodeToDelete->settlementID);
	if (itt != highLevelNodes.end()) {
		//Remove node
		HLPathNode* p = highLevelNodes.at(nodeToDelete->settlementID);
		highLevelNodes.erase(itt);
		delete p;
	}
	//Now delete all road segs associated with this node
	for (int i = 0; i < allRoads.size(); i++) {
		if ((allRoads[i]->HLNodeIDs.first == nodeToDelete->settlementID) || (allRoads[i]->HLNodeIDs.second == nodeToDelete->settlementID)) {
			//One of the IDs is equal to the deletion id
			RoadSegment* r = allRoads[i];
			allRoads.erase(allRoads.begin() + i);
			delete r;
		}
	}
}

HLPathNode * RoadNetwork::getHLNode(Settlement * settlement)
{
	auto itt = highLevelNodes.find(settlement->settlementID);
	if (itt != highLevelNodes.end()) {
		
		return highLevelNodes.at(settlement->settlementID);
	}
	else std::cout << "node not found" << std::endl;
	return nullptr;
}



std::vector<HLPathNode*> RoadNetwork::calcHLNodeNeighbours(HLPathNode * node)
{	
	
	std::vector<HLPathNode*> neighbourNodes;

	//First set all nearby within radius as neighbours
	//If none in radius, set nearest as neighbour
	std::vector<Settlement*> nearby = SettlementManager::getInstance()->getSettlementsInArea(500, node->m_WorldPos.x, node->m_WorldPos.z);
	if (nearby.size() == 0) {
		//Nothing in radius, so just set nearest as neighbour.
		Settlement* nearest = SettlementManager::getInstance()->getNearestSettlement(node->m_WorldPos);
		if (nearest != nullptr) {
			//Just return this one settlement
			neighbourNodes.push_back(highLevelNodes.at(nearest->settlementID));
			return neighbourNodes;
		}
		//No other settlements at all. no neighbours to be set.
		else return neighbourNodes;

	}

	//Get corresponding network nodes for each of these neighbouring settlements.

	for (int i = 0; i < nearby.size(); i++) {
		neighbourNodes.push_back(highLevelNodes.at(nearby[i]->settlementID));
	}
	//Eliminate neighbours based on whether it is faster to go straight there, or through another neighbour (Using weight modifier of for paths through other cities.

	//Try to go to each neighbour through each neighbour
	//Dist == weight
	//If total through neighbour * modifier < straight line
		//remove second as neighbour
	float throughModifier = 0.6f;
	std::vector<int> neighboursToRemove;
	for (int i = 0; i < neighbourNodes.size(); i++) {
		for (int j = 0; j < neighbourNodes.size(); j++) {
			float straightDist = calcDist(node->m_WorldPos, neighbourNodes[j]->m_WorldPos);
			float throughDist = calcDist(node->m_WorldPos, neighbourNodes[i]->m_WorldPos) + calcDist(neighbourNodes[i]->m_WorldPos, neighbourNodes[j]->m_WorldPos);
			if (throughDist != straightDist && (throughDist * throughModifier < straightDist)) {
				//Faster to go through another neighbour
				//Remove
				neighboursToRemove.push_back(neighbourNodes[j]->m_ID);
			}

		}
	}

	//Remove nodes that were calculated to have a better path through other nodes
	for(int i = 0; i < neighboursToRemove.size(); i++) {
		for (int j = 0; j < neighbourNodes.size(); j++) {
			if (neighboursToRemove[i] == neighbourNodes[j]->m_ID) {
				neighbourNodes.erase(neighbourNodes.begin() + j);
			}
		}
	}
	
	//Left with nodes that have no better path through other nodes.
	return neighbourNodes;
}


std::vector<LLPathNode*> RoadNetwork::getPath(glm::vec3 startPos, glm::vec3 endPos)
{
	LLPathNode* start = new LLPathNode(startPos);
	LLPathNode* end = new LLPathNode(endPos);

	allNodes.insert(std::make_pair(start->m_WorldPos, start));
	allNodes.insert(std::make_pair(end->m_WorldPos, end));

	LLPathNode* currentNode = start;
	start->m_LocalGoal = 0.0f;
	start->m_GlobalGoal = calcDist(start->m_WorldPos, end->m_WorldPos);

	//Nodes are added to this as we go for testing.
	std::list<LLPathNode*> untestedNodes;
	untestedNodes.push_back(currentNode);

	while (!untestedNodes.empty()) {

		//Sort so that lowest global is done first.
		untestedNodes.sort([](const LLPathNode* first, const LLPathNode* second) {
			return first->m_GlobalGoal < second->m_GlobalGoal;
		});

		//Get rid of visited nodes at front
		while (!untestedNodes.empty() && untestedNodes.front()->m_IsVisited) {
			untestedNodes.pop_front();
		}

		if (untestedNodes.empty()) break;

		currentNode = untestedNodes.front();
		currentNode->m_IsVisited = true;

		//If near end node
		if (calcDist(currentNode->m_WorldPos, end->m_WorldPos) < 35) {
			std::vector<LLPathNode*> path;
			LLPathNode* pathnode = end;
			end->previousLLNode = currentNode;
			while (pathnode->previousLLNode != nullptr) {

				path.push_back(pathnode);
				pathnode = pathnode->previousLLNode;
			}
			return path;

		}
		
		//Too many nodes with no result
		if (untestedNodes.size() > 600) return std::vector<LLPathNode*>();
		//Temp bug cover
		if (currentNode->m_GlobalGoal > 4 * start->m_GlobalGoal) { return std::vector<LLPathNode*>(); }



		//Try to get neighbouring nodes. If no neighbour node exists, creates a node.
		currentNode->m_Neighbours = getLLNodeNeighbours(currentNode);

		//Check each neighbour
		for (auto neighbour : currentNode->m_Neighbours) {


			if (!neighbour->m_IsVisited && neighbour->m_WorldPos.y > ChunkSettings::SEA_LEVEL) {
				untestedNodes.push_back(neighbour);
			}

			//Here is where new local is calculated
			//Base this off elevation change later. Lower change = better score
			float newLocalGoal = currentNode->m_LocalGoal + calcDist(currentNode->m_WorldPos, neighbour->m_WorldPos);

			if (newLocalGoal < neighbour->m_LocalGoal) {
				//Set goals of this next best neighbour node.
				neighbour->previousLLNode = currentNode;
				neighbour->m_LocalGoal = newLocalGoal;
				neighbour->m_GlobalGoal = neighbour->m_LocalGoal + calcDist(neighbour->m_WorldPos, end->m_WorldPos);
			}
		}
	}

	return std::vector<LLPathNode*>();
}



std::vector<LLPathNode*> RoadNetwork::getLLNodeNeighbours(LLPathNode * currNode)
{
	std::vector<LLPathNode*> neighbours;
	int numNeighbours = 6;
	float neighbourDistance = 35.0f;
	float angleStep = (2 * M_PI) / numNeighbours;

	//Try to get the node at the position of every surrounding neighbour point.
	for (int i = 0; i < numNeighbours; i++) {
		//Position of potential neighbour
		glm::vec3 neighbourNodePos = glm::vec3(currNode->m_WorldPos.x + neighbourDistance * cos(angleStep*i), currNode->m_WorldPos.y, currNode->m_WorldPos.z + neighbourDistance * sin(angleStep*i));

		neighbourNodePos.y = terrainNoise->generateTotalNoise(neighbourNodePos.x, neighbourNodePos.z);
		if (neighbourNodePos.y < ChunkSettings::SEA_LEVEL) continue;

		LLPathNode* neighbour = getLLNode(neighbourNodePos);
		neighbours.push_back(neighbour);
	}

	return neighbours;
}

LLPathNode * RoadNetwork::getLLNode(glm::vec3 pos)
{

	if (allNodes.count(pos) == 0) {
		//No node found, so create.
		LLPathNode* newNode = new LLPathNode(pos);
		allNodes.insert(std::make_pair(pos, newNode));
		
		return newNode;
	}
	//Node exists, return it
	LLPathNode* n = allNodes[pos];
	return n;
}

float RoadNetwork::calcDist(glm::vec3 startPosition, glm::vec3 endPosition)
{
	float xDist = abs(startPosition.x - endPosition.x);
	float yDist = abs(startPosition.y - endPosition.y);
	float zDist = abs(startPosition.z - endPosition.z);

	float res = sqrt((xDist * xDist) + (yDist * yDist) + (zDist * zDist));
	return res;
}


namespace std
{
	bool operator < (const glm::vec3& lhs, const glm::vec3& rhs)
	{
		if (lhs == rhs) return false;
		return std::make_tuple(lhs.x, lhs.y, lhs.z) < std::make_tuple(rhs.x, rhs.y, rhs.z);
	}
}