#include "transform.h"

using namespace Mino;

using TransformRef = Transform::TransformRef;

Transform::~Transform() {}

void Transform::flip()
{
    position = 1 - position;
    children.iterateActive([](auto& tr) { tr.flip(); });
}

TransformRef Transform::addChild()
{
    auto result = children.enable();
    result->self = result;
    result->parent = self;
    return result;
}

void Transform::removeChild(TransformRef const& child) { child.disable(); }

Transform::Transform(Transform const& t)
    : parent(t.parent), position(t.position), rotation(t.rotation), children(t.children)
{
}

Transform::Transform(Transform&& t)
    : parent(t.parent), position(t.position), rotation(t.rotation), children(t.children)
{
}

Transform& Transform::operator=(Transform const& t)
{
    parent = t.parent;
    position = t.position;
    positions = t.positions;
    rotation = t.rotation;
    children = t.children;
    return *this;
}

Transform& Transform::operator=(Transform&& t)
{
    parent = t.parent;
    position = t.position;
    positions = t.positions;
    rotation = t.rotation;
    children = t.children;
    return *this;
}
