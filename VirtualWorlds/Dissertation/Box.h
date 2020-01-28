#pragma once

#include <Object3D.h>

class Box : public Object3D
{
public:
	Box(VAOData* vaoData, GLuint shader);
	~Box();

private:

	void onInit() override;
	void onUpdate() override;
};

