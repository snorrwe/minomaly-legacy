#pragma once
#include "collider.h"
#include "vector2.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Mino
{

class BoxColliderComponent : public ColliderComponent
{
public:
    enum Corner
    {
        TopLeft = 0,
        TopRight = 1,
        BottomLeft = 2,
        BottomRight = 3
    };

    virtual void start();

    double getWidth(double value) const { return width; }
    double getHeight(double value) const { return height; }
    void set(double w, double h);

    virtual void checkCollisions();
    virtual BoundingBox asBoundingBox() const;

protected:
    double width = 0;
    double height = 0;
};

} // namespace Mino