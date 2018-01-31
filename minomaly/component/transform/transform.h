#pragma once
#include "iterable_pool.h"
#include "matrix.h"
#include "renderdata.h"
#include "vector2.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <math.h>
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
    Transform(Transform const&) = delete;
    Transform(Transform&&) = default;
    Transform(TransformRef const& parent) : parent(parent) {}
    virtual ~Transform() {}

    Transform& operator=(Transform const&) = delete;
    Transform& operator=(Transform&&) = default;

    virtual TransformRef addChild(TransformRef&& child = nullptr);
    virtual void removeChild(TransformRef const& child);
    size_t childCount() { return children->enabled(); }

    void updateChildren();
    void updateAsRoot();

    TransformRef::WeakRef getParent() { return parent; }

    Vector& position() { return localTransform.position; }
    Vector const& getPosition() const { return localTransform.position; }
    void setPosition(Vector const& value);

    Vector& scale() { return localTransform.scale; }
    Vector const& getScale() const { return localTransform.scale; }
    void setScale(Vector const& value);

    RotationData& rotation() { return localTransform.rotation; }
    RotationData const& getRotation() const { return localTransform.rotation; }
    void setRotation(RotationData const& value);

    TransformData const& absolute() const { return absoluteTransform; }
    Matrix transformMatrix();

protected:
    TransformRef::WeakRef parent = nullptr;
    TransformRef::WeakRef self = nullptr;
    std::unique_ptr<ChildrenContainer> children = std::make_unique<ChildrenContainer>();

    TransformData localTransform = {};
    TransformData absoluteTransform = {};

private:
    Matrix transformMatrix(TransformData const& parent);

    void updateByParent(TransformData const&);
};

} // namespace Mino
