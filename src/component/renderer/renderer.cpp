#include "renderer.h"

using namespace Mino;

void RendererComponent::enable()
{
    auto myself = std::static_pointer_cast<RendererComponent>(self.lock());
    renderSystem.lock()->enableRenderer(myself);
    Component::enable();
}

void RendererComponent::disable()
{
    auto myself = std::static_pointer_cast<RendererComponent>(self.lock());
    renderSystem.lock()->disableRenderer(myself);
    Component::disable();
}
