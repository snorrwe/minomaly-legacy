#include "box_collider.h"

using namespace Mino;

BoundingBox BoxColliderComponent::asBoundingBox() const { return box; }

void BoxColliderComponent::updateCornersByPosition(Vector2<float> const& position)
{
    auto w = width * transform->absolute().scale.x();
    auto h = height * transform->absolute().scale.y();
    auto center = position + Vector2<float>{w * 0.5f, h * 0.5f} + offset;
    box = {std::move(center), w, h};
    corners.clear();
    corners.push_back({center.x() - w * 0.5f, center.y() - h * 0.5f});
    corners.push_back({center.x() + w * 0.5f, center.y() - h * 0.5f});
    corners.push_back({center.x() - w * 0.5f, center.y() + h * 0.5f});
    corners.push_back({center.x() + w * 0.5f, center.y() + h * 0.5f});
    for (auto& corner : corners)
    {
        auto m = Matrix<1, 3>{{corner.x(), corner.y(), 1.f}};
        auto result = m * transform->transformMatrix();
        corner = {m.at(0, 0), m.at(0, 1)};
    }
}

void BoxColliderComponent::set(float w, float h, Vector2<float> ofs)
{
    offset = std::move(ofs);
    width = w;
    height = h;
}

bool BoxColliderComponent::intersects(ColliderComponent const& coll) const
{
    auto isInsideTheBox = [&](Vector2<float> const& point) {
        auto colliding = false;
        for (int i = 0, j = corners.size() - 1; i < corners.size(); j = i++)
        {
            if (((corners[i].y() > point.y()) != (corners[j].y() > point.y()))
                && (point.x() < (corners[j].x() - corners[i].x()) * (point.y() - corners[i].y())
                                        / (corners[j].y() - corners[i].y())
                                    + corners[i].x()))
            {
                colliding = !colliding;
            }
        }
        return colliding;
    };
    for (auto& vertex : coll.getCorners())
    {
        if (isInsideTheBox(vertex)) return true;
    }
    return false;
}
