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
class Application;

class GameObject
{
public:
    using ComponentContainer = std::vector<std::unique_ptr<Component>>;
    using ChildrenContainer = std::vector<GameObject*>;

    GameObject() = default;
    GameObject(Transform::TransformRef const& parentTransform);
    GameObject(Transform::TransformRef const& parentTransform, Application* application);
    GameObject(GameObject const& go) = delete;
    GameObject(GameObject&& go) = default;
    virtual ~GameObject();

    GameObject& operator=(GameObject const& go) = delete;
    GameObject& operator=(GameObject&& go) = default;

    Transform::TransformRef& getTransform() { return transform; }
    Transform::TransformRef const& getTransform() const { return transform; }

    template <typename TComponent> TComponent* addComponent();
    template <typename TComponent> TComponent* getComponent() const;
    virtual void disableComponent(Component*);
    virtual void enableComponent(Component*);

    virtual void update();
    virtual void addChild(GameObject&);
    virtual void removeChild(GameObject&);

    GameObject* getParent() const { return parent; }

    Application* getApplication() const { return application; }

protected:
    ComponentContainer components = {};
    GameObject* parent = nullptr;
    ChildrenContainer children = {};
    Transform::TransformRef transform = nullptr;
    size_t enabled = 0;
    Application* application = nullptr;
};

template <typename TComponent> TComponent* GameObject::addComponent()
{
    auto component = Component::create<TComponent>(this);
    auto result = component.get();
    components.insert(components.begin() + enabled, std::move(component));
    ++enabled;
    return result;
}

template <typename TComponent> TComponent* GameObject::getComponent() const
{
    for (auto& i : components)
    {
        if (i && typeid(*i) == typeid(TComponent))
        {
            return static_cast<TComponent*>(i.get());
        }
    }
    return nullptr;
}

} // namespace Mino
