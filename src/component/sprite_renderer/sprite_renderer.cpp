#include "sprite_renderer.h"

using namespace Mino;

template <>
std::shared_ptr<SpriteRendererComponent>
Component::create<SpriteRendererComponent>(GameObject* gameObject)
{
    return RendererComponent::create<SpriteRendererComponent>(gameObject);
}

void SpriteRendererComponent::start()
{
    gameObject->getTransform();
    if (!texture) disable();
}

void SpriteRendererComponent::render()
{
    auto& position = transform->getPosition();
    auto x = static_cast<int>(position.x());
    auto y = static_cast<int>(position.y());
    texture->render({x, y});
}

void SpriteRendererComponent::setTexture(std::shared_ptr<Texture> t) { texture = t; }

void SpriteRendererComponent::clearTexture()
{
    texture = nullptr;
    disable();
}
