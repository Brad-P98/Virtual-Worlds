#include "PathNodes.h"

HLPathNode::HLPathNode(int id, glm::vec3 worldPos)
{
	m_ID = id;
	m_WorldPos = worldPos;
}

HLPathNode::~HLPathNode()
{
}

LLPathNode::LLPathNode(glm::vec3 worldPos)
{
	m_WorldPos = worldPos;
	m_IsVisited = false;
	m_GlobalGoal = INFINITY;
	m_LocalGoal = INFINITY;
}

LLPathNode::~LLPathNode()
{
}
