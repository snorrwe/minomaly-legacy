#pragma once
#include "vector2.h"
#include <cmath>

namespace Mino
{

class BoundingBox
{
public:
    BoundingBox(Vector2<double> center, double halfDimension)
        : center(center), halfDimension(halfDimension)
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
    double getHalfDimension() const { return halfDimension; }
    void setCenter(Vector2<double> const& value) { center = value; }
    void setHalfDimension(double value) { halfDimension = value; }

private:
    Vector2<double> center;
    double halfDimension;
};

} // namespace Mino
