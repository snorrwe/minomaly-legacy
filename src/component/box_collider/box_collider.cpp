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
    Vector2<double> center{corners[Corner::TopLeft].x() + width * 0.5,
                           corners[Corner::TopLeft].y() + height * 0.5};
    return {center, width, height};
}

void BoxColliderComponent::set(double w, double h, Vector2<double> of)
{
    removeFromWorld();
    offset = of;
    width = w;
    height = h;
    auto topLeft = transform->getPosition() + offset;
    corners[Corner::TopLeft] = topLeft;
    corners[Corner::TopRight] = {topLeft.x() + width, topLeft.y()};
    corners[Corner::BottomLeft] = {topLeft.x(), topLeft.y() + height};
    corners[Corner::BottomRight] = {topLeft.x() + width, topLeft.y() + height};
    addToWorld();
}

void BoxColliderComponent::checkCollisions()
{
    auto points = world.lock()->queryRange(asBoundingBox());
    if (points.size() > 4)
    {
        std::sort(points.begin(), points.end(),
                  [](auto const& lhs, auto const& rhs) { return lhs.item < rhs.item; });
        points.erase(
            std::unique(points.begin(), points.end(),
                        [](auto const& lhs, auto const& rhs) { return lhs.item == rhs.item; }),
            points.end());
        auto thisIt = std::find_if(points.begin(), points.end(),
                                   [&](auto const& i) { return i.item == this; });
        if (thisIt != points.end()) points.erase(thisIt);

        for (auto i = points.begin(); i != points.end(); ++i)
        {
            i->item->handleCollision(*this);
        }
    }
}
