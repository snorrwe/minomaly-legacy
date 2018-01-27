#pragma once
#include "program.h"

using namespace Mino;

Program::~Program()
{
    auto gameObjectsToDelete = GameObjectContainer{std::move(gameObjects)};
    gameObjects = std::move(GameObjectContainer{});
}

GameObject* Program::createEmptyGameObject()
{
    gameObjects.emplace_back(std::make_unique<GameObject>(rootTransform->addChild(), this));
    return gameObjects.back().get();
}

void Program::destroyGameObject(GameObject* go)
{
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [&](auto& g) { return g.get() == go; });
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

void Program::updateGameObjects()
{
    for (auto& i : gameObjects)
    {
        i->update();
    }
    rootTransform->updateAsRoot();
}

template <> void Program::addComponents<>(GameObject& go) {}

void Program::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<>({0.0, screenHeight});
    camera->addComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}
