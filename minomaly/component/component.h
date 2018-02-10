#pragma once
#include "game_object.h"
#include "transform.h"
#include <memory>
#include <type_traits>

namespace Mino
{
class GameObject;
class IEngineCore;

class Component
{
public:
    template <typename TComponent>
    static std::unique_ptr<TComponent> create(GameObject& gameObject);

    virtual ~Component() {}

    virtual void start() {}
    virtual void update() {}

    virtual void enable();
    virtual void disable();

    bool isEnabled() { return enabled; }

    void setTransform(Transform::TransformRef const& value) { transform = value; }
    Transform::TransformRef& getTransform() { return transform; }
    Transform::TransformRef const& getTransform() const { return transform; }

protected:
    template <typename TComponent>
    static std::unique_ptr<TComponent> create();

    IEngineCore* minomaly = nullptr;
    GameObject* gameObject = nullptr;
    Transform::TransformRef transform = nullptr;
    bool enabled = true;
};

template <typename TComponent>
std::unique_ptr<TComponent> Component::create(GameObject& gameObject)
{
    static_assert(std::is_convertible<TComponent*, Component*>::value,
                  "Components must derive from Component!");
    auto result = create<TComponent>();
    result->gameObject = &gameObject;
    result->setTransform(gameObject.getTransform());
    result->minomaly = gameObject.minomaly();
    result->start();
    return std::move(result);
}

template <typename TComponent>
std::unique_ptr<TComponent> Component::create()
{
    return std::make_unique<TComponent>();
}

} // namespace Mino
