#pragma once
#include "scene.h"

using namespace Mino;

std::shared_ptr<GameObject>& Scene::createEmptyGameObject()
{
    gameObjects.emplace_back(std::make_shared<GameObject>(rootTransform->addChild(), this));
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
    for (auto& i : gameObjects)
    {
        i->update();
    }
    rootTransform->updateChildren();
}

template <> void Scene::addComponents<>(GameObject& go) {}

void Scene::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<>({0.0, screenHeight});
    camera->addComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}
