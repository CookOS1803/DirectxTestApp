#pragma once
#include "NormWin.h"
#include <memory>
#include "SceneObject.h"

class Graphics;

class Scene
{
public:

	constexpr Scene(Graphics* gfx) : objects(), pGfx(gfx) {}

	constexpr const std::vector<std::unique_ptr<SceneObject>>& Objects() const noexcept { return objects; }
	SceneObject* AddObject(const SceneObject& obj);

private:

	std::vector<std::unique_ptr<SceneObject>> objects;
	Graphics* pGfx;
};

