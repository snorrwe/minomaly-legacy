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

TransformRef Transform::addChild(TransformRef&& child)
{
    auto result = children->enable();
    result->self = result;
    result->parent = self;
    if (child)
    {
        result->children = std::move(child->children);
        result->localTransform = std::move(child->localTransform);
        result->absoluteTransform = std::move(child->absoluteTransform);
    }
    result->updateByParent(*this);
    return result;
}

void Transform::removeChild(TransformRef const& child)
{
    if (child.enabled()) child.disable();
}

void Transform::setPosition(Vector const& value) { localTransform.position = value; }

void Transform::setRotation(RotationData const& value) { localTransform.rotation = value; }

void Transform::updateChildren()
{
    if (!children) return; // TODO: this shouldn't happen
    children->iterateActive([&](auto& tr) { tr.updateByParent(*this); });
    children->iterateActive([&](auto& tr) { tr.updateChildren(); });
}

void Transform::updateAsRoot()
{
    absoluteTransform = TransformData(localTransform);
    updateChildren();
}

void Transform::updateByParent(Transform const& parent)
{
    absoluteTransform.position = parent.absoluteTransform.position + localTransform.position;
}
