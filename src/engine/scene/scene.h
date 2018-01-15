#pragma once
#include "core.h"
#include "game_object.h"
#include "iterable_pool.h"
#include "transform.h"
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
    virtual void update() = 0;
    virtual void start() {}
    virtual void flipTransforms();

    template <typename... TComponents> std::shared_ptr<GameObject> createGameObject();
    void destroyGameObject(std::shared_ptr<GameObject>);

    IEngineCore* getEngineCore() { return engine; }

protected:
    std::shared_ptr<GameObject> createEmptyGameObject();
    template <typename TComponent> void addComponent(GameObject& go);
    template <typename... Ts> void addComponents(GameObject& go);
    template <typename T, typename... Ts> void addComponentHelper(GameObject& go);

    Transform::ChildrenContainer rootTransforms = Transform::ChildrenContainer{};
    std::vector<std::shared_ptr<GameObject>> gameObjects;
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

template <typename... TComponents> std::shared_ptr<GameObject> Scene::createGameObject()
{
    auto go = createEmptyGameObject();
    addComponents<TComponents...>(*go);
    return go;
}

} // namespace Mino
