#pragma once
#include "iterable_pool.h"
#include "renderdata.h"
#include "vector2.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

namespace Mino
{

class Transform
{
public:
    using ChildrenContainer = IterablePool<Transform>;
    using TransformRef = ChildrenContainer::Reference;
    using RotationData = Mino::RenderData::RotationData;
    using Vector = Vector2<float>;

    static TransformRef getRoot();

    struct TransformData
    {
        TransformData() = default;
        TransformData(TransformData const&) = default;
        TransformData(TransformData&&) = default;
        ~TransformData() = default;
        TransformData& operator=(TransformData const&) = default;
        TransformData& operator=(TransformData&&) = default;

        Vector position = {0, 0};
        Vector scale = {1.0, 1.0};
        RotationData rotation = {};
    };

    Transform() = default;
    Transform(Transform const&) = default;
    Transform(Transform&&) = default;
    Transform(TransformRef const& parent) : parent(parent) {}
    virtual ~Transform() {}

    Transform& operator=(Transform const&) = default;
    Transform& operator=(Transform&&) = default;

    virtual TransformRef addChild(TransformRef const& child = nullptr);
    virtual void removeChild(TransformRef const& child);
    size_t childCount() { return children.enabled(); }

    void updateChildren();

    TransformRef::WeakRef getParent() { return parent; }

    Vector& position() { return localTransform.position; }
    Vector const& getPosition() const { return localTransform.position; }
    void setPosition(Vector const& value);

    RotationData& rotation() { return localTransform.rotation; }
    RotationData const& getRotation() const { return localTransform.rotation; }
    void setRotation(RotationData const& value);

    TransformData const& absolute() { return absoluteTransform; }

protected:
    TransformRef::WeakRef parent = nullptr;
    TransformRef::WeakRef self = nullptr;
    ChildrenContainer children = {};

    TransformData localTransform = {};
    TransformData absoluteTransform = {};

private:
    void updateByParent(Transform const& parent);
};

} // namespace Mino
