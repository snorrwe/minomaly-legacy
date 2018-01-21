#include "sprite_render.h"

using namespace Mino;

template <>
std::unique_ptr<SpriteRenderComponent>
Component::create<SpriteRenderComponent>(GameObject* gameObject)
{
    return RenderComponent::create<SpriteRenderComponent>(gameObject);
}

void SpriteRenderComponent::start() { transform = gameObject->getTransform(); }

void SpriteRenderComponent::update()
{
    if (!texture && isEnabled()) disable();
}

void SpriteRenderComponent::render(Transform::TransformRef camera)
{
    auto offset = camera ? camera->absolute().position : Vector2<float>{0, 0};
    auto position = transform->absolute().position - offset;
    auto x = static_cast<int>(position.x());
    auto y = static_cast<int>(position.y());
    texture->render({x, -y - height});
}

void SpriteRenderComponent::setTexture(std::shared_ptr<Texture> t)
{
    texture = t;
    height = t->getHeight();
}

void SpriteRenderComponent::clearTexture()
{
    texture = nullptr;
    disable();
}
