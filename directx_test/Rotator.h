#pragma once
#include "Updateable.h"
class Rotator : public Updateable
{
public:
	Rotator(SceneObject* obj);

	// Унаследовано через Updateable
	virtual void Update(float deltaTime) override;

private:

	float timer = 0.f;
};

