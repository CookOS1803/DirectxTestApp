#include "CylinderMovement.h"
#include "SceneObject.h"
#include <cmath>

CylinderMovement::CylinderMovement(SceneObject* obj) : Updateable(obj)
{
}

void CylinderMovement::Update(float deltaTime)
{
	timer += deltaTime;

	p_sceneObject->GetTransform().position.y = 2 * std::sin(timer - XM_PIDIV4);
	p_sceneObject->GetTransform().eulerRotation.z = std::sin(timer - XM_PI);
}
