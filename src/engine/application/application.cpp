#include "application.h"

using namespace Mino;

Application::~Application()
{
    auto gameObjectsToDelete = GameObjectContainer{std::move(gameObjects)};
    gameObjects =GameObjectContainer{};
}

GameObject* Application::createEmptyGameObject()
{
    gameObjects.emplace_back(std::make_unique<GameObject>(rootTransform->addChild(), this));
    return gameObjects.back().get();
}

void Application::destroyGameObject(GameObject* go)
{
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
                           [&](auto& g) { return g.get() == go; });
    if (it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

void Application::updateGameObjects()
{
    for (auto& i : gameObjects)
    {
        i->update();
    }
    rootTransform->updateAsRoot();
}

template <> void Application::addComponents<>(GameObject& go) {}

void Application::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<>({0.0, screenHeight});
    camera->addComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}
