#pragma once
#include "component.h"
#include "transform.h"
#include <algorithm>
#include <iostream>
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
    typedef std::vector<std::shared_ptr<Component>> ComponentContainer;

    GameObject() = default;
    virtual ~GameObject() {}

    std::shared_ptr<Transform> getTransform() const { return transform; }

    template <typename TComponent> std::shared_ptr<TComponent> addComponent();
    template <typename TComponent> std::shared_ptr<TComponent> getComponent() const;
    void disableComponent(std::shared_ptr<Component>);
    void enableComponent(std::shared_ptr<Component>);

    void update();

protected:
    std::shared_ptr<Transform> transform;
    ComponentContainer components;
    size_t enabled = 0;
};

template <typename TComponent> std::shared_ptr<TComponent> GameObject::addComponent()
{
    auto result = Component::create<TComponent>(this);
    components.insert(components.begin() + enabled, result);
    ++enabled;
    return result;
}

template <typename TComponent> std::shared_ptr<TComponent> GameObject::getComponent() const
{
    for (auto i = components.begin(); i != components.end(); ++i)
    {
        if (*i && typeid(**i) == typeid(TComponent))
        {
            return std::static_pointer_cast<TComponent>(*i);
        }
    }
    return nullptr;
}

} // namespace Mino
