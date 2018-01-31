#include "box_collider.h"

using namespace Mino;

BoundingBox BoxColliderComponent::asBoundingBox() const { return box; }

void BoxColliderComponent::updateCornersByPosition(Vector2<float> const& position)
{
    auto w = width * transform->absolute().scale.x();
    auto h = height * transform->absolute().scale.y();
    auto center = position + Vector2<float>{w * 0.5f, h * 0.5f} + offset;
    box = {std::move(center), w, h};
}

void BoxColliderComponent::set(float w, float h, Vector2<float> ofs)
{
    offset = std::move(ofs);
    width = w;
    height = h;
}
