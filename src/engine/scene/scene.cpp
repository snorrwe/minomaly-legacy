#pragma once
#include "scene.h"

using namespace Mino;

GameObject& Scene::createGameObject()
{
    gameObjects.emplace_back(rootTransform);
    return gameObjects.back();
}

void Scene::destroyGameObject(GameObject& go)
{
    auto it =
        std::find_if(gameObjects.begin(), gameObjects.end(), [&](auto& g) { return &g == &go; });
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}
