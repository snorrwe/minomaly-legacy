#include "sprite_renderer.h"

using namespace Mino;

template <>
std::unique_ptr<SpriteRendererComponent>
Component::create<SpriteRendererComponent>(GameObject* gameObject)
{
    return RenderComponent::create<SpriteRendererComponent>(gameObject);
}

void SpriteRendererComponent::start() { transform = gameObject->getTransform(); }

void SpriteRendererComponent::update()
{
    if (!texture && isEnabled()) disable();
}

void SpriteRendererComponent::render(Transform::TransformRef camera)
{
    auto offset = camera ? camera->absolute().position : Vector2<float>{0, 0};
    auto& absolute = transform->absolute();
    auto position = absolute.position - offset;
    auto x = static_cast<int>(position.x());
    auto y = static_cast<int>(position.y());
    texture->render({{x, -y - height}, absolute.scale, absolute.rotation});
}

void SpriteRendererComponent::setTexture(Texture* t)
{
    if (!t) return;
    texture = t;
    height = t->getHeight();
}

void SpriteRendererComponent::clearTexture()
{
    texture = nullptr;
    disable();
}
