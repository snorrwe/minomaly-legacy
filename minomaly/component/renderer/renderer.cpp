#include "renderer.h"

using namespace Mino;

void RenderComponent::enable()
{
    renderSystem->enableRenderer(this);
    Component::enable();
}

void RenderComponent::disable()
{
    renderSystem->disableRenderer(this);
    Component::disable();
}

RenderComponent::~RenderComponent()
{
    if (renderSystem) renderSystem->removeRenderer(this);
}
