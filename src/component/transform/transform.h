#pragma once
#include "renderdata.h"
#include "vector2.h"
#include <algorithm>
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

protected:
    Transform* parent = nullptr;
    Vector2<> position = Vector2<>{};
    RotationData rotation = RotationData{};
    ChildrenContainer children = ChildrenContainer{};

private:
    size_t id = 0;
    size_t childId = 0;
};

} // namespace Mino
