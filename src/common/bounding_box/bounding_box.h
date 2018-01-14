#pragma once
#include "vector2.h"
#include <cmath>

namespace Mino
{

class BoundingBox
{
public:
    BoundingBox(Vector2<double> center, double halfDimension)
        : center(center), halfWidth(halfDimension), halfHeight(halfDimension)
    {
    }
    BoundingBox(Vector2<double> center, double width, double height)
        : center(center), halfWidth(width * 0.5), halfHeight(height * 0.5)
    {
    }
    BoundingBox(BoundingBox const&) = default;
    BoundingBox(BoundingBox&&) = default;
    ~BoundingBox() {}

    BoundingBox& operator=(BoundingBox const&) = default;
    BoundingBox& operator=(BoundingBox&&) = default;

    bool containsPoint(Vector2<double> const& point) const;
    bool intersects(BoundingBox const& other) const;

    Vector2<double> getCenter() const { return center; }
    double getWidth() { return halfWidth * 2; }
    double getHeight() { return halfHeight * 2; }

    void setCenter(Vector2<double> const& value) { center = value; }
    void setWidth(double value) { halfWidth = value * 0.5; }
    void setHeight(double value) { halfHeight = value * 0.5; }

private:
    Vector2<double> center;
    double halfWidth;
    double halfHeight;
};

} // namespace Mino
