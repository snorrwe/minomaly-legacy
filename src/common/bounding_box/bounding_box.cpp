#include "bounding_box.h"

using namespace Mino;

bool BoundingBox::containsPoint(Vector2<double> const& point) const
{
    return center.x() - halfDimension <= point.x() && point.x() <= center.x() + halfDimension &&
           center.y() - halfDimension <= point.y() && point.y() <= center.y() + halfDimension;
}

bool BoundingBox::intersects(BoundingBox const& other) const
{ // TODO: test this!
    auto o_top = other.center.y() + other.halfDimension;
    auto o_bottom = other.center.y() - other.halfDimension;
    if (center.y() + halfDimension <= o_top || center.y() - halfDimension >= o_bottom) return false;

    auto o_right = other.center.x() + other.halfDimension;
    auto o_left = other.center.x() - other.halfDimension;
    if (center.x() - halfDimension >= o_right || center.x() + halfDimension <= o_left) return false;

    return true;
}
