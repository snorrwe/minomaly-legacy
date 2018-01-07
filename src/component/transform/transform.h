#pragma once
#include "renderdata.h"
#include "vector2.h"
#include <algorithm>
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
    using ChildrenContainer = std::vector<Transform>;
    using RotationData = Mino::RenderData::RotationData;
    using Vector = Vector2<double>;

    static std::shared_ptr<Transform> create(std::shared_ptr<Transform> parent = nullptr);

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

    void setPosition(Vector const& value) { position = value; }
    void setPosition(double const x, double const y) { position = Vector{x, y}; }
    Vector& getPosition() { return position; }
    Vector const& getPosition() const { return position; }
    void setRotation(RotationData const& value) { rotation = value; }
    RotationData& getRotation() { return rotation; }
    RotationData const& getRotation() const { return rotation; }

protected:
    Transform* parent = nullptr;
    Vector position = Vector{};
    RotationData rotation = RotationData{};
    ChildrenContainer children = ChildrenContainer{};

private:
    size_t id = 0;
    size_t childId = 0;
};

} // namespace Mino
