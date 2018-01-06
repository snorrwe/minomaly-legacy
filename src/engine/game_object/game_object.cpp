#include "game_object.h"

using namespace Mino;

void GameObject::disableComponent(std::shared_ptr<Component> component)
{
    auto last = components.begin() + enabled;
    auto target = std::find(components.begin(), last, component);
    if (target != last)
    {
        if (components.size() > enabled)
            std::iter_swap(target, --last);
        else
            std::iter_swap(target, components.rbegin());
        --enabled;
    }
}

void GameObject::enableComponent(std::shared_ptr<Component> component)
{
    auto first = components.begin() + enabled;
    auto target = std::find(first, components.end(), component);
    if (target != components.end())
    {
        std::iter_swap(first, target);
        ++enabled;
    }
}

void GameObject::update()
{
    auto toUpdate = components;
    auto last = toUpdate.begin() + enabled;
    for (auto i = toUpdate.begin(); i != last; ++i)
    {
        (*i)->update();
    }
}
