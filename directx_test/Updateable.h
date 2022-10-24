#pragma once

class Updateable
{
public:

	Updateable(class SceneObject* sceneObject);

	virtual void Update(float deltaTime) = 0;

protected:

	SceneObject* p_sceneObject;
};