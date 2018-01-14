#include "bounding_box.h"

using namespace Mino;

bool BoundingBox::containsPoint(Vector2<double> const& point) const
{
    return center.x() - halfWidth <= point.x() && point.x() <= center.x() + halfWidth &&
           center.y() - halfHeight <= point.y() && point.y() <= center.y() + halfHeight;
}

bool BoundingBox::intersects(BoundingBox const& other) const
{
    return (fabs(center.x() - other.center.x()) < halfWidth + other.halfWidth) &&
           (fabs(center.y() - other.center.y()) < halfHeight + other.halfHeight);
}
