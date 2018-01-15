#pragma once
#include "collider.h"

namespace Mino
{

class BoxCollider : public Collider
{
public:
    virtual void update();
    virtual bool intersects(Collider const&) const;
    virtual BoundingBox asBoundingBox() const;

    double getWidth(double value) const { return width; }
    double getHeight(double value) const { return height; }

    void setWidth(double value) { width = value; }
    void setHeight(double value) { height = value; }

    void set(double w, double h)
    {
        width = w;
        height = h;
    }

protected:
    double width = 0;
    double height = 0;
    BoundingBox box = BoundingBox({0, 0}, 0, 0);
};

} // namespace Mino
