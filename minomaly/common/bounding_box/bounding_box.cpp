#include "bounding_box.h"

using namespace Mino;

bool BoundingBox::containsPoint(Vector2<float> const& point) const
{
    return center.x() - halfWidth <= point.x() && point.x() <= center.x() + halfWidth
           && center.y() - halfHeight <= point.y() && point.y() <= center.y() + halfHeight;
}

bool BoundingBox::intersects(BoundingBox const& other) const
{
    return (fabs(center.x() - other.center.x()) < halfWidth + other.halfWidth)
           && (fabs(center.y() - other.center.y()) < halfHeight + other.halfHeight);
}

void BoundingBox::set(Vector2<float> const& bottomLeft, Vector2<float> const& topRight)
{
    auto width = topRight.x() - bottomLeft.x();
    auto height = topRight.y() - bottomLeft.y();

    halfWidth = fabs(width) * 0.5f;
    halfHeight = fabs(height) * 0.5f;
    center = bottomLeft + Vector2<float>{halfWidth, halfHeight};
}
