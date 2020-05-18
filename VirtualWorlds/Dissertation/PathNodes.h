#pragma once
#include <glm/glm/glm.hpp>
#include <vector>

//Low-Level path node - Stores nodes during A* (essentially road segments)
class LLPathNode
{
public:

	LLPathNode(glm::vec3 worldPos);
	~LLPathNode();

	glm::vec3 m_WorldPos;
	bool m_IsVisited;

	float m_GlobalGoal;
	float m_LocalGoal;

	LLPathNode* previousLLNode = nullptr;
	std::vector<LLPathNode*> m_Neighbours;

};


//High-level PathNode - Stores a settlement location & neighbours (Topological structure)
class HLPathNode
{
public:

	HLPathNode(int id, glm::vec3 worldPos);
	~HLPathNode();

	int m_ID;
	//Contain every possible starting point for this settlement
	glm::vec3 m_WorldPos;
	std::vector<glm::vec3> connectingPoints;
	//Each of these neighbours will have some 'offshoots' that can be pathed to as end points
	std::vector<HLPathNode*> m_Neighbours;
};

