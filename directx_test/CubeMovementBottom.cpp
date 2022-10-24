#include "CubeMovementBottom.h"
#include "SceneObject.h"
#include <cmath>

CubeMovementBottom::CubeMovementBottom(SceneObject* obj) : Updateable(obj)
{
}

void CubeMovementBottom::Update(float deltaTime)
{
	timer += deltaTime;

	p_sceneObject->GetTransform().eulerRotation.y = -timer;
	p_sceneObject->GetTransform().position.x = 5 * std::cos(timer) + 5.f;
	p_sceneObject->GetTransform().position.y = -5.f * std::abs(std::sin(timer));
	p_sceneObject->GetTransform().position.z = -5.f * std::sin(timer);
}
