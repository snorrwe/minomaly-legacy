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
    friend class PhysicsSystem;

    using ChildrenContainer = IterablePool<Transform>;
    using TransformRef = ChildrenContainer::Reference;
    using RotationData = Mino::RenderData::RotationData;
    using Vector = Vector2<float>;

    Transform() = default;
    Transform(Transform const&);
    Transform(Transform&&);
    Transform(TransformRef parent) : parent(parent) {}
    virtual ~Transform();

    Transform& operator=(Transform const&);
    Transform& operator=(Transform&&);

    virtual TransformRef addChild();
    virtual void removeChild(TransformRef const& child);
    size_t childCount() { return children.enabled(); }

    TransformRef getParent() { return parent; }

    void setPosition(Vector const& value) { position = value; }
    void setPosition(float const x, float const y) { position = Vector{x, y}; }
    Vector& getPosition() { return position; }
    Vector const& getPosition() const { return position; }
    void setRotation(RotationData const& value) { rotation = value; }
    RotationData& getRotation() { return rotation; }
    RotationData const& getRotation() const { return rotation; }

protected:
    TransformRef parent = nullptr;
    TransformRef self = nullptr;
    Vector position;
    RotationData rotation = RotationData{};
    ChildrenContainer children = ChildrenContainer{};
};

} // namespace Mino
