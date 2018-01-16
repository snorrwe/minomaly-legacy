#pragma once
#include "scene.h"

using namespace Mino;

void Scene::flipTransforms()
{
    rootTransforms.iterateActive([](auto& t) {
        t.flip();
        t.reset();
    });
}

std::shared_ptr<GameObject> Scene::createEmptyGameObject()
{
    gameObjects.emplace_back(std::make_shared<GameObject>(rootTransforms.enable(), this));
    return gameObjects.back();
}

void Scene::destroyGameObject(std::shared_ptr<GameObject> go)
{
    auto it =
        std::find_if(gameObjects.begin(), gameObjects.end(), [&](auto& g) { return &g == &go; });
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

void Scene::updateGameObjects()
{
    for (auto i = gameObjects.begin(); i != gameObjects.end(); ++i)
    {
        (**i).update();
    }
}

template <> void Scene::addComponents<>(GameObject& go) {}
