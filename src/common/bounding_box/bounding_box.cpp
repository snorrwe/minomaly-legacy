#include "bounding_box.h"

using namespace Mino;

bool BoundingBox::containsPoint(Vector2<double> const& point) const
{
    return center.x() - halfDimension <= point.x() && point.x() <= center.x() + halfDimension &&
           center.y() - halfDimension <= point.y() && point.y() <= center.y() + halfDimension;
}

bool BoundingBox::intersects(BoundingBox const& other) const { return false; }
