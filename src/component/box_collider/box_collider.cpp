#include "box_collider.h"

using namespace Mino;

void BoxColliderComponent::start()
{
    corners.reserve(4);
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
    return {center, width - 0.1f, height - 0.1f};
}

void BoxColliderComponent::set(float w, float h, Vector2<float> ofs)
{
    removeFromWorld();
    offset = std::move(ofs);
    width = w;
    height = h;
    auto topLeft = transform->absolute().position + offset;
    auto x = topLeft.x(), y = topLeft.y();
    corners[Corner::BottomLeft] = std::move(topLeft);
    corners[Corner::BottomRight] = {x + width, y};
    corners[Corner::TopLeft] = {x, y + height};
    corners[Corner::TopRight] = {x + width, y + height};
    addToWorld();
}
