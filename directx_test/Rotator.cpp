#include "Rotator.h"
#include "SceneObject.h"

Rotator::Rotator(SceneObject* obj) : Updateable(obj)
{
}

void Rotator::Update(float deltaTime)
{
	timer += deltaTime;

	p_sceneObject->GetTransform().eulerRotation.y = timer;
}
