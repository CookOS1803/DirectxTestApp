#pragma once
#include "Updateable.h"

class CubeMovementBottom : public Updateable
{
public:

	CubeMovementBottom(SceneObject* obj);

	// Унаследовано через Updateable
	virtual void Update(float deltaTime) override;

private:

	float timer = 0.f;
};

