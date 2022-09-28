#pragma once
#include "NormWin.h"
#include "SceneObject.h"

class Scene
{
public:

	constexpr Scene() : objects() {}

	constexpr const std::vector<SceneObject>& Objects() const noexcept { return objects; }
	SceneObject* AddObject(const SceneObject& obj);

private:

	std::vector<SceneObject> objects;
};

