#include "TerrainBehaviour.h"
#include <iostream>



TerrainBehaviour::TerrainBehaviour()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

}


TerrainBehaviour::~TerrainBehaviour()
{
}

void TerrainBehaviour::update()
{
	worldPos = Instance::m_scene->getMainCamera()->getWorldPos();

	std::cout << worldPos.x << " " << worldPos.y << " " << worldPos.z << std::endl;
	//Check if any terrain must be removed

	//Check if terrain must be added


}
