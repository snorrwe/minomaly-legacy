#include "transform.h"

using namespace Mino;

using TransformRef = Transform::TransformRef;

TransformRef Transform::getRoot()
{
    static Transform::ChildrenContainer roots = {};

    auto result = roots.enable();
    result->self = result;
    return result;
}

TransformRef Transform::addChild(TransformRef const& child)
{
    auto result = children.enable();
    result->self = result;
    result->parent = self;
    if (child)
    {
        result->children = child->children;
        result->localTransform = child->localTransform;
        result->absoluteTransform = child->absoluteTransform;
    }
    result->updateByParent(*this);
    return result;
}

void Transform::removeChild(TransformRef const& child) {if(child.enabled()) child.disable(); }

void Transform::setPosition(Vector const& value) { localTransform.position = value; }

void Transform::setRotation(RotationData const& value) { localTransform.rotation = value; }

void Transform::updateChildren()
{
    if (!parent)
    {
        absoluteTransform = TransformData(localTransform);
    }
    children.iterateActive([&](auto& tr) {
        tr.updateByParent(*this);
        tr.updateChildren();
    });
}

void Transform::updateByParent(Transform const& parent)
{
    absoluteTransform.position = parent.absoluteTransform.position + localTransform.position;
}
