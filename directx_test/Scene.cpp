#include "Scene.h"
#include "Graphics.h"

SceneObject* Scene::AddObject(const SceneObject& obj)
{
    objects.emplace_back(std::make_unique<SceneObject>(obj));

    auto p = objects.back().get();

    pGfx->AddObject(p);

    return p;
}
