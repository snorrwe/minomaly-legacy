#pragma once
#include "bounding_box.h"
#include "vector2.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Mino
{

class Quadtree
{
public:
    static const size_t capacity = 4;
    using Vector = Vector2<double>;
    using Points = std::vector<Vector>;

    Quadtree(BoundingBox const& boundary = BoundingBox({0, 0}, 0), Quadtree* parent = nullptr)
        : boundary(boundary), parent(parent)
    {
    }
    Quadtree(Quadtree const&) = default;
    Quadtree(Quadtree&&) = default;
    ~Quadtree() {}

    Quadtree& operator=(Quadtree const&) = default;
    Quadtree& operator=(Quadtree&&) = default;

    bool insert(Vector const& v);
    bool contains(Vector const& v);
    bool move(Vector const& from, Vector const& to);
    bool erase(Vector const& v);

    std::vector<Vector> queryRange(BoundingBox const& range);

protected:
    bool query(Vector const& v, std::function<bool(Quadtree&)> callback);
    void subdivide();
    bool propagateUp(Vector const& v);

private:
    BoundingBox boundary;
    Quadtree* parent;
    Points points = Points{};

    std::unique_ptr<Quadtree> northWest = nullptr;
    std::unique_ptr<Quadtree> northEast = nullptr;
    std::unique_ptr<Quadtree> southWest = nullptr;
    std::unique_ptr<Quadtree> southEast = nullptr;
};

} // namespace Mino
