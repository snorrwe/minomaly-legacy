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

template <class T> class Quadtree
{
public:
    struct Node
    {
        Node() = default;
        Node(float x, float y, T* item = nullptr) : pos({x, y}), item(item) {}
        Node(Vector2<float> pos, T* item = nullptr) : pos(pos), item(item) {}
        Node(Node const&) = default;
        Node(Node&&) = default;
        Node& operator=(Node const&) = default;
        Node& operator=(Node&&) = default;

        bool operator==(Node const& n) const { return pos == n.pos && item == n.item; }
        bool operator!=(Node const& n) const { return !(*this == n); }

        Vector2<float> pos;
        T* item = nullptr;
    };

    using Points = std::vector<Node>;

    static const size_t capacity = 64;

    Quadtree(BoundingBox const& boundary = BoundingBox({0, 0}, 0), Quadtree* parent = nullptr)
        : boundary(boundary), parent(parent)
    {
    }
    Quadtree(Quadtree const&) = default;
    Quadtree(Quadtree&&) = default;
    ~Quadtree() {}

    Quadtree& operator=(Quadtree const&) = default;
    Quadtree& operator=(Quadtree&&) = default;

    bool insert(Node const& v);
    bool contains(Node const& v);
    bool move(Node const& from, Node const& to);
    bool erase(Node const& v);

    std::vector<Node> queryRange(BoundingBox const& range);
    void queryRange(BoundingBox const& range, std::vector<Node>& result);

protected:
    bool query(Node const& v, std::function<bool(Quadtree&)> callback);
    void subdivide();
    bool propagateUp(Node const& v);

private:
    BoundingBox boundary;
    Quadtree* parent;
    Points points = Points{};

    std::unique_ptr<Quadtree> northWest = nullptr;
    std::unique_ptr<Quadtree> northEast = nullptr;
    std::unique_ptr<Quadtree> southWest = nullptr;
    std::unique_ptr<Quadtree> southEast = nullptr;
};

template <class T> bool Quadtree<T>::insert(typename Quadtree<T>::Node const& v)
{
    if (!boundary.containsPoint(v.pos)) return false;

    if (points.size() < capacity)
    {
        points.push_back(v);
        return true;
    }

    if (northWest == nullptr) subdivide();

    if (northWest->insert(v)) return true;
    if (northEast->insert(v)) return true;
    if (southWest->insert(v)) return true;
    if (southEast->insert(v)) return true;

    throw std::runtime_error("Point cannot be inserted. This should never happen!");
}

template <class T> void Quadtree<T>::subdivide()
{
    auto subDimension = boundary.getWidth() * 0.5f;
    auto center = boundary.getCenter();
    northWest = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() + subDimension}, subDimension), this);
    northEast = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() + subDimension}, subDimension), this);
    southWest = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() - subDimension}, subDimension), this);
    southEast = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() - subDimension}, subDimension), this);
}

template <class T> bool Quadtree<T>::contains(typename Quadtree<T>::Node const& v)
{
    return query(v, [&](Quadtree& qt) {
        return std::find(qt.points.begin(), qt.points.end(), v) != qt.points.end();
    });
}

template <class T> bool Quadtree<T>::propagateUp(typename Quadtree<T>::Node const& v)
{
    if (boundary.containsPoint(v.pos)) return insert(v);
    if (!parent) return false;
    return parent->propagateUp(v);
}

template <class T>
bool Quadtree<T>::move(typename Quadtree<T>::Node const& from, typename Quadtree<T>::Node const& to)
{
    if (from == to) return true;
    auto i = std::find(points.begin(), points.end(), from);
    if (i != points.end())
    {
        points.erase(i);
        if (propagateUp(to)) return true;
    }
    return false;
}

template <class T> bool Quadtree<T>::erase(typename Quadtree<T>::Node const& v)
{
    return query(v, [&](auto& qt) {
        auto i = std::find(qt.points.begin(), qt.points.end(), v);
        auto result = i != qt.points.end();
        if (result) qt.points.erase(i);
        return result;
    });
}

template <class T>
bool Quadtree<T>::query(typename Quadtree<T>::Node const& node,
                        std::function<bool(Quadtree<T>&)> callback)
{
    if (callback(*this)) return true;
    if (!northWest) return false;
    auto center = boundary.getCenter();
    auto& v = node.pos;
    if (v.x() < center.x())
    {
        if (v.y() > center.y())
            return northWest->query(node, callback);
        else if (v.y() < center.y())
            return southWest->query(node, callback);
        return northWest->query(node, callback) || southWest->query(node, callback);
    }
    else if (v.x() > center.x())
    {
        if (v.y() > center.y())
            return northEast->query(node, callback);
        else if (v.y() < center.y())
            return southEast->query(node, callback);
        return northEast->query(node, callback) || southEast->query(node, callback);
    }
    // v.x == center.x ==> could be on either edge
    if (v.y() < center.y())
        return southWest->query(node, callback) || southEast->query(node, callback);
    else if (v.y() > center.y())
        return northWest->query(node, callback) || northEast->query(node, callback);
    // v == center ==> it could be in either of these
    return (northWest->query(node, callback) || northEast->query(node, callback) ||
            southWest->query(node, callback) || southEast->query(node, callback));
}

template <class T>
std::vector<typename Quadtree<T>::Node> Quadtree<T>::queryRange(BoundingBox const& range)
{
    auto result = std::vector<Node>{};
    queryRange(range, result);
    return result;
}

template <class T>
void Quadtree<T>::queryRange(BoundingBox const& range,
                             std::vector<typename Quadtree<T>::Node>& result)
{
    if (!boundary.intersects(range)) return;

    std::copy_if(points.begin(), points.end(), std::back_inserter(result),
                 [&](auto const& i) { return range.containsPoint(i.pos); });

    if (northWest)
    {
        northWest->queryRange(range, result);
        northEast->queryRange(range, result);
        southWest->queryRange(range, result);
        southEast->queryRange(range, result);
    }
}

} // namespace Mino
