#include "Scene.h"
#include "Graphics.h"

SceneObject* Scene::AddObject(const SceneObject& obj)
{
    objects.emplace_back(std::make_unique<SceneObject>(obj));

    auto p = objects.back().get();

    pGfx->AddObject(p);

    return p;
}

SceneObject* Scene::CreateObject()
{
    objects.emplace_back(std::make_unique<SceneObject>());

    auto p = objects.back().get();

    pGfx->AddObject(p);

    return p;
}
