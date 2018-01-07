#pragma once
#include "core.h"
#include "game_object.h"
#include "transform.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Mino
{

class Core;

class Scene
{
public:
    Scene(std::shared_ptr<Core> engine) : engine(engine.get()) {}
    virtual ~Scene() {}

    void updateGameObjects();
    virtual void update() = 0;
    virtual void start() {}

    std::shared_ptr<GameObject> createGameObject();
    void destroyGameObject(std::shared_ptr<GameObject>);

    Core* getEngineCore() { return engine; }
    std::shared_ptr<Transform> getRootTransform() { return rootTransform; }

protected:
    std::shared_ptr<Transform> rootTransform = Transform::create();
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    Core* engine;
};

} // namespace Mino
