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

    double getWidth() const { return width; }
    double getHeight() const { return height; }
    void set(double w, double h, Vector2<double> offset = {0, 0});

    virtual void checkCollisions();
    virtual BoundingBox asBoundingBox() const;

protected:
    Vector2<double> offset = {0, 0};
    double width = 0;
    double height = 0;
};

} // namespace Mino
