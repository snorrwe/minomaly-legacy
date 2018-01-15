#pragma once
#include "bounding_box.h"
#include "component.h"
#include "transform.h"

namespace Mino
{

class Collider : public Component
{
public:
    virtual ~Collider() {}

    virtual void start();
    virtual void update();

    virtual bool overlapping(Collider const&) const;
    virtual bool intersects(Collider const&) const = 0;
    virtual BoundingBox asBoundingBox() const = 0;

protected:
    Transform::TransformRef transform;
};

} // namespace Mino
