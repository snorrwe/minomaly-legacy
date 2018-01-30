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
    result->updateByParent(absoluteTransform);
    return result;
}

void Transform::removeChild(TransformRef const& child)
{
    if (child.enabled()) child.disable();
}

void Transform::setPosition(Vector const& value) { localTransform.position = value; }

void Transform::setScale(Vector const& value) { localTransform.scale = value; }

void Transform::setRotation(RotationData const& value) { localTransform.rotation = value; }

void Transform::updateChildren()
{
    if (!children) return; // TODO: this shouldn't happen
    children->foreachActive([&](auto& tr) { tr.updateByParent(absoluteTransform); });
    children->foreachActive([&](auto& tr) { tr.updateChildren(); });
}

void Transform::updateAsRoot()
{
    absoluteTransform = TransformData(localTransform);
    updateChildren();
}

void Transform::updateByParent(TransformData const& parent)
{
    auto parentMatrix = transformMatrix(parent);
    auto position = Matrix({{localTransform.position.x(), localTransform.position.y(), 1.0f}});

    auto positionMatrix = Matrix::dot(position, parentMatrix);
    absoluteTransform.position = {positionMatrix[0][0], positionMatrix[0][1]};
    absoluteTransform.scale = {parent.scale.x() * localTransform.scale.x(),
                               parent.scale.y() * localTransform.scale.y()};
    absoluteTransform.rotation = parent.rotation + localTransform.rotation;
}

Matrix Transform::transformMatrix(TransformData const& data)
{
    const auto cx = cos(data.rotation.angle) * data.scale.x();
    const auto sy = sin(data.rotation.angle) * data.scale.y();
    return Matrix({cx, -sy, data.position.x(), sy, cx, data.position.y(), 0, 0, 1}, 3, 3);
}
