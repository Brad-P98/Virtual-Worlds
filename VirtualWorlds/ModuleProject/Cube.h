#pragma once

#include "Object3D.h"


class Cube : public Object3D
{
public:

	Cube(VAOData* vaoData, GLuint shader);
	~Cube();

private:

	//These methods are called by object3d, not by the user
	void onInit() override;	//Called after default object initialization
	void onUpdate() override; //Called before default object update

};

