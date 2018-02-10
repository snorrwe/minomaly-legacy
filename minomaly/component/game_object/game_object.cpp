#include "game_object.h"
#include "application.h" // TODO

using namespace Mino;
using std::iter_swap;

GameObject::GameObject(Transform::TransformRef const& parentTransform) : transform(parentTransform)
{
}

GameObject::GameObject(Transform::TransformRef const& parentTransform, IEngineCore* engine)
    : transform(parentTransform),
      engineCore(engine),
      application(engine ? engine->getApplication() : nullptr)
{
}

GameObject::~GameObject()
{
    if (application)
    {
        for (auto child : children)
        {
            child->parent = nullptr;
            application->destroyGameObject(child);
        }
    }
    if (parent)
    {
        parent->removeChild(*this);
    }
}

IEngineCore* GameObject::minomaly() const { return engineCore; }

Application* GameObject::getApplication() const { return application; }

void GameObject::disableComponent(Component* component)
{
    auto last = components.begin() + enabled;
    auto target
        = std::find_if(components.begin(), last, [&](auto& i) { return i.get() == component; });
    if (target != last)
    {
        if (components.size() > enabled)
        {
            iter_swap(target, --last);
        }
        else
        {
            iter_swap(target, components.rbegin());
        }
        --enabled;
    }
}

void GameObject::enableComponent(Component* component)
{
    auto first = components.begin() + enabled;
    auto target
        = std::find_if(first, components.end(), [&](auto& i) { return i.get() == component; });
    if (target != components.end())
    {
        iter_swap(first, target);
        ++enabled;
    }
}

void GameObject::update()
{
    if (!components.size()) return;
    static auto toUpdate = std::vector<Component*>{};
    toUpdate.reserve(components.size());
    std::transform(components.begin(), components.end(), std::back_inserter(toUpdate),
                   [](auto& i) { return i.get(); });
    auto last = toUpdate.begin() + enabled;
    for (auto i = toUpdate.begin(); i != last; ++i)
    {
        (*i)->update();
    }
    toUpdate.clear();
}

void GameObject::addChild(GameObject& go)
{
    if (std::find(children.begin(), children.end(), &go) == children.end())
    {
        go.transform = transform->addChild(std::move(go.transform));
        children.push_back(&go);
        go.parent = this;
        for (auto& component : go.components)
        {
            component->setTransform(go.transform);
        }
    }
}

void GameObject::removeChild(GameObject& go)
{
    auto it = std::find(children.begin(), children.end(), &go);
    if (it != children.end())
    {
        transform->removeChild(go.transform);
        go.parent = nullptr;
        children.erase(it);
        if (application)
            go.transform = application->getRootTransform();
        else
            go.transform = nullptr;
        for (auto& component : go.components)
        {
            component->setTransform(go.transform);
        }
    }
}
