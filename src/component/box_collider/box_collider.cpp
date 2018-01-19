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

void BoxColliderComponent::checkCollisions()
{
    auto points = world.lock()->queryRange(asBoundingBox());
    if (points.size() <= 4) return;

    std::sort(points.begin(), points.end(),
              [](auto const& lhs, auto const& rhs) { return lhs.item < rhs.item; });
    points.erase(std::unique(points.begin(), points.end(),
                             [](auto const& lhs, auto const& rhs) { return lhs.item == rhs.item; }),
                 points.end());
    removeSelf(points);
    for (auto& i : points)
    {
        i.item->handleCollision(*this);
    }
}

void BoxColliderComponent::removeSelf(std::vector<World::Node>& points)
{
    auto it =
        std::find_if(points.begin(), points.end(), [&](auto const& i) { return i.item == this; });
    if (it != points.end()) points.erase(it);
}
