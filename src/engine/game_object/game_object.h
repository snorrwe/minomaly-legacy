#pragma once
#include "component.h"
#include "transform.h"
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace Mino
{

class Component;

class GameObject
{
public:
    GameObject() = default;
    virtual ~GameObject() {}

    Transform getTransform() { return transform; }

    template <typename TComponent> std::shared_ptr<TComponent> addComponent();
    template <typename TComponent> std::shared_ptr<TComponent> getComponent() const;

protected:
    Transform transform;
    std::vector<std::shared_ptr<Component>> components;
};

template <typename TComponent> std::shared_ptr<TComponent> GameObject::addComponent()
{
    auto result = Component::create<TComponent>(this);
    components.push_back(result);
    return result;
}

template <typename TComponent> std::shared_ptr<TComponent> GameObject::getComponent() const
{
    for (auto i = components.begin(); i != components.end(); ++i)
    {
        if (typeid(**i) == typeid(TComponent))
        {
            return std::static_pointer_cast<TComponent>(*i);
        }
    }
    return nullptr;
}

} // namespace Mino
