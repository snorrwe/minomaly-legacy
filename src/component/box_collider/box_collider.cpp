#include "box_collider.h"

using namespace Mino;

void BoxCollider::update() { box = BoundingBox(transform->getPosition(), width, height); }

bool BoxCollider::intersects(Collider const& c) const
{
    return asBoundingBox().intersects(c.asBoundingBox());
}

BoundingBox BoxCollider::asBoundingBox() const { return box; }
