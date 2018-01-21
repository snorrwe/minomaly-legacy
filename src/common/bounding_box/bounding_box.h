#pragma once
#include "vector2.h"
#include <cmath>

namespace Mino
{

class BoundingBox
{
public:
    BoundingBox(Vector2<float> center, float halfDimension)
        : center(center), halfWidth(halfDimension), halfHeight(halfDimension)
    {
    }
    BoundingBox(Vector2<float> center, float width, float height)
        : center(center), halfWidth(width * 0.5), halfHeight(height * 0.5)
    {
    }
    BoundingBox(BoundingBox const&) = default;
    BoundingBox(BoundingBox&&) = default;
    ~BoundingBox() {}

    BoundingBox& operator=(BoundingBox const&) = default;
    BoundingBox& operator=(BoundingBox&&) = default;

    bool containsPoint(Vector2<float> const& point) const;
    bool intersects(BoundingBox const& other) const;

    Vector2<float> getCenter() const { return center; }
    float getWidth() { return halfWidth * 2.0f; }
    float getHeight() { return halfHeight * 2.0f; }

    void setCenter(Vector2<float> const& value) { center = value; }
    void setWidth(float value) { halfWidth = value * 0.5f; }
    void setHeight(float value) { halfHeight = value * 0.5f; }

private:
    Vector2<float> center;
    float halfWidth;
    float halfHeight;
};

} // namespace Mino
