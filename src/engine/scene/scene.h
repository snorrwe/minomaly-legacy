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

    virtual void update() = 0;
    virtual void start() {}

    GameObject& createGameObject();
    void destroyGameObject(GameObject&);

    Core* getEngineCore() { return engine; }

protected:
    std::shared_ptr<Transform> rootTransform = Transform::create();
    std::vector<GameObject> gameObjects;
    Core* engine;
};

} // namespace Mino
