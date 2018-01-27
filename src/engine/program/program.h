#pragma once
#include "core.h"
#include "game_object.h"
#include "iterable_pool.h"
#include "render_system.h"
#include "transform.h"
#include "vector2.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Mino
{

class IEngineCore;
class IRenderSystem;
class GameObject;

class Program
{
public:
    using GameObjectContainer = std::vector<std::unique_ptr<GameObject>>;

    Program(std::shared_ptr<IEngineCore> engine) : engine(engine.get()) {}
    Program(Program const&) = delete;
    Program(Program&&) = delete;
    virtual ~Program();

    Program& operator=(Program const&) = delete;
    Program& operator=(Program&&) = delete;

    void updateGameObjects();
    virtual void update() {}
    virtual void start() {}

    template <typename... TComponents>
    GameObject* createGameObject(Vector2<float> position = {0, 0});
    void destroyGameObject(GameObject*);

    GameObject* getMainCamera() { return mainCamera; }
    void initMainCamera(IRenderSystem const& renderer, float screenHeight);

    IEngineCore* getEngineCore() { return engine; }
    Transform::TransformRef getRootTransform() { return rootTransform->addChild(); }

protected:
    GameObject* createEmptyGameObject();
    template <typename TComponent> void addComponent(GameObject& go);
    template <typename... Ts> void addComponents(GameObject& go);
    template <typename T, typename... Ts> void addComponentHelper(GameObject& go);

    Transform::TransformRef rootTransform = Transform::getRoot();
    GameObjectContainer gameObjects;
    GameObject* mainCamera;
    IEngineCore* engine;
};

template <typename TComponent> void Program::addComponent(GameObject& go)
{
    go.addComponent<TComponent>();
}

template <typename... Ts> void Program::addComponents(GameObject& go)
{
    addComponentHelper<Ts...>(go);
}

template <> void Program::addComponents<>(GameObject& go);

template <typename T, typename... Ts> void Program::addComponentHelper(GameObject& go)
{
    addComponent<T>(go);
    addComponents<Ts...>(go);
}

template <typename... TComponents> GameObject* Program::createGameObject(Vector2<float> position)
{
    auto go = createEmptyGameObject();
    auto tr = go->getTransform();
    tr->setPosition(position);
    tr->updateAsRoot();
    addComponents<TComponents...>(*go);
    return go;
}

} // namespace Mino
