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
    width = w;
    height = h;
    auto topRight = transform->getPosition();
    corners[0] = topRight;
    corners[1] = {topRight.x() + width, topRight.y()};
    corners[2] = {topRight.x(), topRight.y() + height};
    corners[3] = {topRight.x() + width, topRight.y() + height};
}

void BoxColliderComponent::checkCollisions()
{ /*TODO*/
}
