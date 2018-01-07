#include "renderer.h"

using namespace Mino;

void RenderComponent::enable()
{
    auto myself = std::static_pointer_cast<RenderComponent>(self.lock());
    renderSystem.lock()->enableRenderer(myself);
    Component::enable();
}

void RenderComponent::disable()
{
    auto myself = std::static_pointer_cast<RenderComponent>(self.lock());
    renderSystem.lock()->disableRenderer(myself);
    Component::disable();
}
