#include "renderer.h"

using namespace Mino;

void RenderComponent::enable()
{
    renderSystem.lock()->enableRenderer(*this);
    Component::enable();
}

void RenderComponent::disable()
{
    renderSystem.lock()->disableRenderer(*this);
    Component::disable();
}
