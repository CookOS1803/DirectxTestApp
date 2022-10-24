#pragma once
#include "Updateable.h"

class CylinderMovement : public Updateable
{
public:

	CylinderMovement(SceneObject* obj);


	virtual void Update(float deltaTime) override;

private:

	float timer = 0.f;
};

