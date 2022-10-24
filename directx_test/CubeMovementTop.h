#pragma once
#include "Updateable.h"

class CubeMovementTop : public Updateable
{
public:

	CubeMovementTop(SceneObject* obj);

	// ������������ ����� Updateable
	virtual void Update(float deltaTime) override;

private:

	float timer = 0.f;
};

