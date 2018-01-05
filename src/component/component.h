#pragma once
#include "game_object.h"
#include <memory>
#include <type_traits>

namespace Mino
{

class GameObject;

class Component
{
public:
    template <typename TComponent>
    static std::shared_ptr<TComponent> create(GameObject* gameObject);

    virtual ~Component() {}

    virtual void start() {}
    virtual void update() {}

protected:
    GameObject* gameObject = nullptr;
};

template <typename TComponent> std::shared_ptr<TComponent> Component::create(GameObject* gameObject)
{
    static_assert(std::is_convertible<TComponent*, Component*>::value);
    auto result = std::make_shared<TComponent>();
    result->gameObject = gameObject;
    return result;
}

} // namespace Mino
