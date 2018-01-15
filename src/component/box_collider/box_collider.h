#pragma once
#include "collider.h"
#include "vector2.h"

namespace Mino
{

class BoxColliderComponent : public ColliderComponent
{
public:
    virtual void start();

    double getWidth(double value) const { return width; }
    double getHeight(double value) const { return height; }
    void set(double w, double h);

    virtual void checkCollisions();

protected:
    double width = 0;
    double height = 0;
};

} // namespace Mino
