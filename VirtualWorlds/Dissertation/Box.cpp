#include "Box.h"


Box::Box(VAOData* vaoData, GLuint shader)
{
	init(vaoData, shader);
}


Box::~Box()
{
}

void Box::onInit()
{

	transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -5));

}

void Box::onUpdate()
{
}
