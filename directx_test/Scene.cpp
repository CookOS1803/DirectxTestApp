#include "Scene.h"
#include "Graphics.h"

SceneObject* Scene::CreateObject()
{
    objects.emplace_back(std::make_unique<SceneObject>());

    return objects.back().get();
}

SceneObject* Scene::CreateUIObject()
{
    uiObjects.emplace_back(std::make_unique<SceneObject>());

    return uiObjects.back().get();
}
