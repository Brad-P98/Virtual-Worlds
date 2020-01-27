#include "Cube.h"

Cube::Cube(VAOData* vaoData, GLuint shader)
{
	init(vaoData, shader);

}

Cube::~Cube()
{
}

//Override. Called after init
void Cube::onInit()
{
	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5));
}

//Override
void Cube::onUpdate()
{

}




