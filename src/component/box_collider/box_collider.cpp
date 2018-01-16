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

void BoxColliderComponent::set(double w, double h)
{
    removeFromWorld();
    width = w;
    height = h;
    auto& topRight = transform->getPosition();
    corners[0] = topRight;
    corners[1] = {topRight.x() + width, topRight.y()};
    corners[2] = {topRight.x(), topRight.y() + height};
    corners[3] = {topRight.x() + width, topRight.y() + height};
    addToWorld();
}

void BoxColliderComponent::checkCollisions()
{
    Vector2<double> center{corners[0].x() + width * 0.5, corners[0].y() + height * 0.5};
    auto points = world.lock()->queryRange({center, width, height});
    if (points.size() > 4)
    {
        std::sort(points.begin(), points.end(),
                  [](auto const& lhs, auto const& rhs) { return lhs.item < rhs.item; });
        points.erase(
            std::unique(points.begin(), points.end(),
                        [](auto const& lhs, auto const& rhs) { return lhs.item == rhs.item; }),
            points.end());
        points.erase(std::find_if(points.begin(), points.end(),
                                  [&](auto const& i) { return i.item == this; }));

        for (auto i = points.begin(); i != points.end(); ++i)
        {
            i->item->handleCollision(*this);
        }
    }
}
