#pragma once
#include "game_object.h"
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
    static std::unique_ptr<TComponent> create(GameObject* gameObject);

    virtual ~Component() {}

    virtual void start() {}
    virtual void update() {}

    virtual void enable();
    virtual void disable();

    bool isEnabled() { return enabled; }

protected:
    template <typename TComponent> static std::unique_ptr<TComponent> create();

    GameObject* gameObject = nullptr;
    bool enabled = true;
};

template <typename TComponent> std::unique_ptr<TComponent> Component::create(GameObject* gameObject)
{
    static_assert(std::is_convertible<TComponent*, Component*>::value,
                  "Components must derive from Component!");
    auto result = create<TComponent>();
    result->gameObject = gameObject;
    result->start();
    return std::move(result);
}

template <typename TComponent> std::unique_ptr<TComponent> Component::create()
{
    return std::make_unique<TComponent>();
}

} // namespace Mino
