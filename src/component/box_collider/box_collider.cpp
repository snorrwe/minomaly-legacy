#include "box_collider.h"

using namespace Mino;

void BoxColliderComponent::start()
{
    for (int i = 0; i < 4; ++i)
    {
        corners.push_back({0, 0});
    }
    ColliderComponent::start();
}

BoundingBox BoxColliderComponent::asBoundingBox() const
{
    Vector2<float> center{corners[Corner::BottomLeft].x() + width * 0.5f,
                          corners[Corner::BottomLeft].y() + height * 0.5f};
    return {center, width, height};
}

void BoxColliderComponent::set(float w, float h, Vector2<float> ofs)
{
    removeFromWorld();
    offset = ofs;
    width = w;
    height = h;
    auto topLeft = transform->absolute().position + offset;
    auto x = topLeft.x(), y = topLeft.y();
    corners[Corner::BottomLeft] = topLeft;
    corners[Corner::BottomRight] = {x + width, y};
    corners[Corner::TopLeft] = {x, y + height};
    corners[Corner::TopRight] = {x + width, y + height};
    addToWorld();
}
