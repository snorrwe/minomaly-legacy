#include "sprite_renderer.h"

using namespace Mino;

template <>
std::unique_ptr<SpriteRendererComponent>
Component::create<SpriteRendererComponent>(GameObject& gameObject)
{
    return RenderComponent::create<SpriteRendererComponent>(gameObject);
}

void SpriteRendererComponent::update()
{
    if (!texture && isEnabled()) disable();
}

void SpriteRendererComponent::render(Transform::TransformRef const& camera)
{
    auto& absolute = transform->absolute();
    auto position = absolute.position;
    auto cameraMatrix = [&]() { // TODO: Messes up scaling
        const auto cx = camera->absolute().scale.x() * cos(camera->absolute().rotation.angle);
        const auto sy = camera->absolute().scale.y() * sin(camera->absolute().rotation.angle);
        return FixedMatrix<3, 3>({cx, sy, -camera->absolute().position.x(), -sy, cx,
                                  -camera->absolute().position.y(), 0, 0, 1});
    }();
    auto m = FixedMatrix<1, 3>{{position.x(), position.y(), 1}} * cameraMatrix;
    position = {m.at(0, 0), m.at(0, 1)};

    auto sx = absolute.scale.x() / (camera ? camera->absolute().scale.x() : 1.0f);
    auto sy = absolute.scale.y() / (camera ? camera->absolute().scale.y() : 1.0f);
    auto rotation = camera->absolute().rotation - absolute.rotation;
    texture->render({{ position.x(), (-position.y() - (height * sy))}, {sx, sy}, rotation});
}

void SpriteRendererComponent::setTexture(Texture* t)
{
    if (!t) return;
    texture = t;
    height = t->getHeight();
    width = t->getWidth();
}

void SpriteRendererComponent::clearTexture()
{
    texture = nullptr;
    disable();
}
