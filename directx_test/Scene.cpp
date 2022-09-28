#include "Scene.h"

SceneObject* Scene::AddObject(const SceneObject& obj)
{
    objects.push_back(obj);

    return &objects.back();
}
