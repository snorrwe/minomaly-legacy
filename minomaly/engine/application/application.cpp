#include "application.h"
#include "json.h"

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
    const auto predicate = [&go](auto& g) { return g.get() == go; };
    if (auto it = std::find_if(gameObjects.begin(), gameObjects.end(), predicate);
        it != gameObjects.end())
    {
        gameObjects.erase(it);
    }
}

void Application::updateGameObjects()
{
    for (auto& go : gameObjects)
    {
        go->update();
    }
    rootTransform->updateAsRoot();
}

template <>
void Application::addComponents<>(GameObject&)
{
}

void Application::initMainCamera(IRenderSystem const& renderer, float screenHeight)
{
    auto camera = createGameObject<CameraComponent>({0.0, screenHeight});
    camera->getComponent<CameraComponent>()->setCamera(renderer.getMainCamera());
    mainCamera = camera;
}

void Application::unloadScene()
{
    getEngineCore()->getAssets()->collectGarbage();
    gameObjects.clear();
}

Scene const& Application::loadScene(std::string const& path)
{
    unloadScene();
    auto file = std::ifstream(path);
    auto result = Json::parse<Scene>(file);
    currentScene = std::make_unique<Scene>(std::move(result));
    return *currentScene;
}

