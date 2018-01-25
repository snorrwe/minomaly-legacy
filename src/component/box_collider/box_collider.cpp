#include "box_collider.h"

using namespace Mino;

BoundingBox BoxColliderComponent::asBoundingBox() const { return box; }

void BoxColliderComponent::updateCornersByPosition(Vector2<float> const& position)
{
    auto center = position + Vector2<float>{width * 0.5f, height * 0.5f} + offset;
    box = {std::move(center), width, height};
}

void BoxColliderComponent::set(float w, float h, Vector2<float> ofs)
{
    offset = std::move(ofs);
    width = w;
    height = h;
}
