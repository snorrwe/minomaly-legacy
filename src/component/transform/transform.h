#pragma once
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

    using ChildrenContainer = std::vector<Transform>;
    using RotationData = Mino::RenderData::RotationData;
    using Vector = Vector2<double>;

    static std::shared_ptr<Transform> create(Transform* parent = nullptr);

    Transform() = delete;
    Transform(Transform const&);
    Transform(Transform&&);
    Transform(Transform* parent) : parent(parent) {}
    virtual ~Transform() = default;

    Transform& operator=(Transform const&);
    Transform& operator=(Transform&&);

    virtual Transform& addChild();
    virtual Transform& addChild(Transform& child);
    virtual Transform& getChild(size_t count);
    virtual void removeChild(Transform& child);
    std::vector<Transform> getChildren() { return children; }
    size_t childCount() { return children.size(); }

    Transform* getParent() { return parent; }

    void setPosition(Vector const& value) { positions[1 - position] = value; }
    void setPosition(double const x, double const y) { positions[1 - position] = Vector{x, y}; }
    Vector& getPosition() { return positions[position]; }
    Vector const& getPosition() const { return positions[position]; }
    void setRotation(RotationData const& value) { rotation = value; }
    RotationData& getRotation() { return rotation; }
    RotationData const& getRotation() const { return rotation; }

protected:
    void flip() { position = 1 - position; }

    Transform* parent = nullptr;
    uint8_t position = 0;
    std::array<Vector, 2> positions{};
    RotationData rotation = RotationData{};
    ChildrenContainer children = ChildrenContainer{};

private:
    size_t id = 0;
    size_t childId = 0;
};

} // namespace Mino
