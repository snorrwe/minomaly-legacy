#pragma once
#include "vector2.h"
#include <cmath>
#include <iostream>

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

    void set(Vector2<float> const& bottomLeft, Vector2<float> const& topRight);

    Vector2<float> const& getCenter() const { return center; }
    float getWidth() const { return halfWidth * 2.0f; }
    float getHeight() const { return halfHeight * 2.0f; }

    void setCenter(Vector2<float> const& value) { center = value; }
    void setWidth(float value) { halfWidth = value * 0.5f; }
    void setHeight(float value) { halfHeight = value * 0.5f; }

private:
    Vector2<float> center;
    float halfWidth;
    float halfHeight;
};

} // namespace Mino
