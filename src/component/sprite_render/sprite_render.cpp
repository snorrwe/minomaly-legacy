#include "sprite_render.h"

using namespace Mino;

template <>
std::shared_ptr<SpriteRenderComponent>
Component::create<SpriteRenderComponent>(GameObject* gameObject)
{
    return RenderComponent::create<SpriteRenderComponent>(gameObject);
}

void SpriteRenderComponent::start() { transform = gameObject->getTransform(); }

void SpriteRenderComponent::update()
{
    if (!texture && isEnabled()) disable();
}

void SpriteRenderComponent::render()
{
    auto& position = transform->getPosition();
    auto x = static_cast<int>(position.x());
    auto y = static_cast<int>(position.y());
    texture->render({x, y});
}

void SpriteRenderComponent::setTexture(std::shared_ptr<Texture> t) { texture = t; }

void SpriteRenderComponent::clearTexture()
{
    texture = nullptr;
    disable();
}
