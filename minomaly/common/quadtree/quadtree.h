#pragma once
#include "bounding_box.h"
#include "vector2.h"
#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Mino
{
namespace QuadtreeProperties
{
enum Direction
{
    NorthWest = 0,
    NorthEast = 1,
    SouthWest = 2,
    SouthEast = 3,
};

struct Node
{
    Node() = default;
    Node(float x, float y, T* item = nullptr) : pos({x, y}), item(item) {}
    Node(Vector2<float> pos, T* item = nullptr) : pos(pos), item(item) {}
    Node(Node const&) = default;
    Node(Node&&) = default;
    Node& operator=(Node const&) = default;
    Node& operator=(Node&&) = default;

    bool operator==(Node const& n) const
    {
        constexpr auto marginForError = 0.001f;
        return (pos - n.pos).length() < marginForError && (!item || !n.item || item == n.item);
    }
    bool operator!=(Node const& n) const { return !(*this == n); }

    Vector2<float> pos;
    T* item = nullptr;
};

} // namespace QuadtreeProperties

template <class T>
class Quadtree
{
public:
    using Node = QuadtreeProperties::Node;
    using Points = std::vector<Node>;
    using Direction = QuadtreeProperties::Direction;

    Quadtree(BoundingBox const& boundary = BoundingBox({0, 0}, 0), Quadtree* parent = nullptr,
             const size_t capacity = 4)
        : boundary(boundary), parent(parent), capacity(capacity),
    {
        points.reserve(capacity);
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
    void clear();

    std::vector<Node> queryRange(BoundingBox const& range) const;
    void queryRange(BoundingBox const& range, std::vector<Node>& result) const;
    void queryRange(BoundingBox const& range, std::function<void(Node const&)>) const;

protected:
    bool query(Node const& v, std::function<bool(Quadtree&)> callback);
    void subdivide();
    bool propagateUp(Node const& v);

private:
    BoundingBox boundary;
    Quadtree* parent;
    Points points = Points{};
    size_t capacity;

    std::array<std::unique_ptr<Quadtree>, 4> children = {};
};

template <class T>
void Quadtree<T>::clear()
{
    points.clear();
    children[Direction::NorthWest] = nullptr;
    children[Direction::NorthEast] = nullptr;
    children[Direction::SouthWest] = nullptr;
    children[Direction::SouthEast] = nullptr;
}

template <class T>
bool Quadtree<T>::insert(typename Quadtree<T>::Node const& v)
{
    if (!boundary.containsPoint(v.pos)) return false;

    if (children[Direction::NorthWest])
    {
        if (children[Direction::NorthWest]->insert(v)) return true;
        if (children[Direction::NorthEast]->insert(v)) return true;
        if (children[Direction::SouthWest]->insert(v)) return true;
        if (children[Direction::SouthEast]->insert(v)) return true;
    }

    if (points.size() < capacity)
    {
        points.push_back(v);
    }
    else
    {
        subdivide();
        insert(v);
    }
    return true;
}

template <class T>
void Quadtree<T>::subdivide()
{
    const auto subDimension = boundary.getWidth() * 0.5001f;
    const auto center = boundary.getCenter();
    children[Direction::NorthWest] = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() + subDimension}, subDimension), this,
        capacity);
    children[Direction::NorthEast] = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() + subDimension}, subDimension), this,
        capacity);
    children[Direction::SouthWest] = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() - subDimension}, subDimension), this,
        capacity);
    children[Direction::SouthEast] = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() - subDimension}, subDimension), this,
        capacity);
}

template <class T>
bool Quadtree<T>::contains(typename Quadtree<T>::Node const& v)
{
    return query(v, [&](Quadtree& qt) {
        return std::find(qt.points.begin(), qt.points.end(), v) != qt.points.end();
    });
}

template <class T>
bool Quadtree<T>::propagateUp(typename Quadtree<T>::Node const& v)
{
    if (insert(v)) return true;
    if (!parent) return false;
    return parent->propagateUp(v);
}

template <class T>
bool Quadtree<T>::move(typename Quadtree<T>::Node const& from, typename Quadtree<T>::Node const& to)
{
    if (from == to) return true;
    return query(from, [&](auto& qt) {
        auto it = std::find(qt.points.begin(), qt.points.end(), from);
        if (it != qt.points.end())
        {
            qt.points.erase(it);
            return qt.propagateUp(to);
        }
        return false;
    });
}

template <class T>
bool Quadtree<T>::erase(typename Quadtree<T>::Node const& v)
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
    if (!children[Direction::NorthWest]) return false;
    auto& center = boundary.getCenter();
    auto& v = node.pos;
    if (v.x() < center.x())
    {
        if (v.y() > center.y())
            return children[Direction::NorthWest]->query(node, callback);
        else if (v.y() < center.y())
            return children[Direction::SouthWest]->query(node, callback);
        return children[Direction::NorthWest]->query(node, callback)
               || children[Direction::SouthWest]->query(node, callback);
    }
    else if (v.x() > center.x())
    {
        if (v.y() > center.y())
            return children[Direction::NorthEast]->query(node, callback);
        else if (v.y() < center.y())
            return children[Direction::SouthEast]->query(node, callback);
        return children[Direction::NorthEast]->query(node, callback)
               || children[Direction::SouthEast]->query(node, callback);
    }
    // v.x == center.x ==> could be on either edge
    if (v.y() < center.y())
        return children[Direction::SouthWest]->query(node, callback)
               || children[Direction::SouthEast]->query(node, callback);
    else if (v.y() > center.y())
        return children[Direction::NorthWest]->query(node, callback)
               || children[Direction::NorthEast]->query(node, callback);
    // v == center ==> it could be in either of these
    return (children[Direction::NorthWest]->query(node, callback)
            || children[Direction::NorthEast]->query(node, callback)
            || children[Direction::SouthWest]->query(node, callback)
            || children[Direction::SouthEast]->query(node, callback));
}

template <class T>
std::vector<typename Quadtree<T>::Node> Quadtree<T>::queryRange(BoundingBox const& range) const
{
    auto result = std::vector<Node>{};
    result.reserve(128);
    queryRange(range, result);
    return result;
}

template <class T>
void Quadtree<T>::queryRange(BoundingBox const& range,
                             std::vector<typename Quadtree<T>::Node>& result) const
{
    if (!boundary.intersects(range)) return;

    queryRange(range, [&result](auto const& i) { result.push_back(i); });

    if (children[Direction::NorthWest])
    {
        children[Direction::NorthWest]->queryRange(range, result);
        children[Direction::NorthEast]->queryRange(range, result);
        children[Direction::SouthWest]->queryRange(range, result);
        children[Direction::SouthEast]->queryRange(range, result);
    }
}

template <class T>
void Quadtree<T>::queryRange(BoundingBox const& range,
                             std::function<void(typename Quadtree<T>::Node const&)> callback) const
{
    if (!boundary.intersects(range)) return;

    for (auto& point : points)
    {
        callback(point);
    }

    if (children[Direction::NorthWest])
    {
        children[Direction::NorthWest]->queryRange(range, callback);
        children[Direction::NorthEast]->queryRange(range, callback);
        children[Direction::SouthWest]->queryRange(range, callback);
        children[Direction::SouthEast]->queryRange(range, callback);
    }
}

} // namespace Mino
