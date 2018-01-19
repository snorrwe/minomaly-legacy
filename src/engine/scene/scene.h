#pragma once
#include "core.h"
#include "game_object.h"
#include "iterable_pool.h"
#include "transform.h"
#include "vector2.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Mino
{

class IEngineCore;

class Scene
{
public:
    Scene(std::shared_ptr<IEngineCore> engine) : engine(engine.get()) {}
    Scene(Scene const&) = delete;
    Scene(Scene&&) = delete;
    virtual ~Scene() {}

    Scene& operator=(Scene const&) = delete;
    Scene& operator=(Scene&&) = delete;

    void updateGameObjects();
    virtual void update() {}
    virtual void start() {}

    template <typename... TComponents>
    std::shared_ptr<GameObject> createGameObject(Vector2<float> position = {0, 0});
    void destroyGameObject(std::shared_ptr<GameObject>);

    std::shared_ptr<GameObject> getMainCamera() { return mainCamera; }
    void setMainCamera(std::shared_ptr<GameObject> value) { mainCamera = value; }

    IEngineCore* getEngineCore() { return engine; }
    Transform::TransformRef getRootTransform() { return rootTransforms.enable(); }

protected:
    std::shared_ptr<GameObject> createEmptyGameObject();
    template <typename TComponent> void addComponent(GameObject& go);
    template <typename... Ts> void addComponents(GameObject& go);
    template <typename T, typename... Ts> void addComponentHelper(GameObject& go);

    Transform::ChildrenContainer rootTransforms = {};
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::shared_ptr<GameObject> mainCamera;
    IEngineCore* engine;
};

template <typename TComponent> void Scene::addComponent(GameObject& go)
{
    go.addComponent<TComponent>();
}

template <typename... Ts> void Scene::addComponents(GameObject& go)
{
    addComponentHelper<Ts...>(go);
}

template <> void Scene::addComponents<>(GameObject& go);

template <typename T, typename... Ts> void Scene::addComponentHelper(GameObject& go)
{
    addComponent<T>(go);
    addComponents<Ts...>(go);
}

template <typename... TComponents>
std::shared_ptr<GameObject> Scene::createGameObject(Vector2<float> position)
{
    auto go = createEmptyGameObject();
    auto tr = go->getTransform();
    tr->setPosition(position);
    addComponents<TComponents...>(*go);
    return go;
}

} // namespace Mino
