#include "collider.h"

using namespace Mino;

void Collider::start() { transform = gameObject->getTransform(); }

void Collider::update() {}

bool Collider::overlapping(Collider const& c) const { return intersects(c) && c.intersects(*this); }
