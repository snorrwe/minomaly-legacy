#include "collider.h"

using namespace Mino;

void Collider::start() { transform = gameObject->getTransform(); }

void Collider::enable()
{
    /*TODO: enable this in a collection (physicssystem?)*/
    Component::enable();
}

void Collider::disable()
{
    /*TODO: disable this in a collection (physicssystem?)*/
    Component::disable();
}

bool Collider::overlapping(Collider const& c) const { return intersects(c) && c.intersects(*this); }
