#include "application.h"
#include "json.h"
#include <iostream>
#include <string>

using namespace Mino;

Application::~Application()
{
    /*
    Move gameobjects into a different container,
    so the gameObjects container is not mutated by
    destroyGameObject calls
    */
    auto gameObjectsToDelete = std::move(gameObjects);
    gameObjects = GameObjectContainer{};
}

GameObject* Application::createEmptyGameObject()
{
    gameObjects.emplace_back(new GameObject{rootTransform->addChild(), engine});
    return gameObjects.back().get();
}

void Application::destroyGameObject(GameObject* go)
{
    const auto predicate = [&](auto& g) { return g.get() == go; };
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(), predicate);
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

template <>
void Application::addComponents<>(GameObject& go)
{
}

void Application::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<CameraComponent>({0.0, screenHeight});
    camera->getComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}

Scene const& Application::loadScene(std::string const& path)
{
    auto file = std::ifstream(path);
    auto result = Json::parse<Scene>(file);
    // auto stream = std::string{};
    // file >> stream;
    // std::cout << stream << "\n";
    // auto result = Json::parse<Scene>(stream.begin(), stream.end());
    return Scene{}; // TODO
}

