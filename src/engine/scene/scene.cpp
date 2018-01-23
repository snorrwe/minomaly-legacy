#pragma once
#include "scene.h"

using namespace Mino;

Scene::~Scene()
{
    auto gameObjectsToDelete = GameObjectContainer{std::move(gameObjects)};
    gameObjects = std::move(GameObjectContainer{});
}

GameObject* Scene::createEmptyGameObject()
{
    gameObjects.emplace_back(std::make_unique<GameObject>(rootTransform->addChild(), this));
    return gameObjects.back().get();
}

void Scene::destroyGameObject(GameObject* go)
{
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [&](auto& g) { return g.get() == go; });
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

void Scene::updateGameObjects()
{
    for (auto& i : gameObjects)
    {
        i->update();
    }
    rootTransform->updateAsRoot();
}

template <> void Scene::addComponents<>(GameObject& go) {}

void Scene::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<>({0.0, screenHeight});
    camera->addComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}
