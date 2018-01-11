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
class Scene;

class GameObject
{
public:
    typedef std::vector<std::shared_ptr<Component>> ComponentContainer;

    GameObject() = default;
    GameObject(std::shared_ptr<Transform> transform) : transform(Transform::create(transform.get()))
    {
    }
    GameObject(std::shared_ptr<Transform> transform, Scene* scene)
        : transform(Transform::create(transform.get())), scene(scene)
    {
    }
    GameObject(GameObject const& go) = delete;
    GameObject(GameObject&& go) = default;
    virtual ~GameObject() {}

    GameObject& operator=(GameObject const& go) = delete;
    GameObject& operator=(GameObject&& go) = default;

    std::shared_ptr<Transform> getTransform() const { return transform; }

    template <typename TComponent> std::shared_ptr<TComponent> addComponent();
    template <typename TComponent> std::shared_ptr<TComponent> getComponent() const;
    virtual void disableComponent(std::shared_ptr<Component>);
    virtual void enableComponent(std::shared_ptr<Component>);

    virtual void update();

    Scene* getScene() { return scene; }

protected:
    ComponentContainer components = ComponentContainer{};
    size_t enabled = 0;
    std::shared_ptr<Transform> transform = Transform::create(nullptr);
    Scene* scene = nullptr;
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
