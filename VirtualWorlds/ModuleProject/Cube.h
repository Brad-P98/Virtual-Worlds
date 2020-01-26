#pragma once
#include "Object3D.h"


class Cube : public Object3D
{
public:

	Cube(VAOData* vaoData);
	~Cube();

	void update() override;

};

