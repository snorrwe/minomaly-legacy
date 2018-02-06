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

        bool operator==(Node const& n) const
        {
            const auto marginForError = 0.001f;
            return (pos - n.pos).length() < marginForError && (!item || !n.item || item == n.item);
        }
        bool operator!=(Node const& n) const { return !(*this == n); }

        Vector2<float> pos;
        T* item = nullptr;
    };

    using Points = std::vector<Node>;

    Quadtree(BoundingBox const& boundary = BoundingBox({0, 0}, 0), Quadtree* parent = nullptr,
             const size_t capacity = 4)
        : boundary(boundary), parent(parent), capacity(capacity)
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

    std::unique_ptr<Quadtree> northWest = nullptr;
    std::unique_ptr<Quadtree> northEast = nullptr;
    std::unique_ptr<Quadtree> southWest = nullptr;
    std::unique_ptr<Quadtree> southEast = nullptr;
};

template <class T> void Quadtree<T>::clear()
{
    points.clear();
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
}

template <class T> bool Quadtree<T>::insert(typename Quadtree<T>::Node const& v)
{
    if (!boundary.containsPoint(v.pos)) return false;

    if (northWest)
    {
        if (northWest->insert(v)) return true;
        if (northEast->insert(v)) return true;
        if (southWest->insert(v)) return true;
        if (southEast->insert(v)) return true;
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

template <class T> void Quadtree<T>::subdivide()
{
    const auto subDimension = boundary.getWidth() * 0.5001f;
    const auto center = boundary.getCenter();
    northWest = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() + subDimension}, subDimension), this,
        capacity);
    northEast = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() + subDimension}, subDimension), this,
        capacity);
    southWest = std::make_unique<Quadtree>(
        BoundingBox({center.x() - subDimension, center.y() - subDimension}, subDimension), this,
        capacity);
    southEast = std::make_unique<Quadtree>(
        BoundingBox({center.x() + subDimension, center.y() - subDimension}, subDimension), this,
        capacity);
}

template <class T> bool Quadtree<T>::contains(typename Quadtree<T>::Node const& v)
{
    return query(v, [&](Quadtree& qt) {
        return std::find(qt.points.begin(), qt.points.end(), v) != qt.points.end();
    });
}

template <class T> bool Quadtree<T>::propagateUp(typename Quadtree<T>::Node const& v)
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
    auto& center = boundary.getCenter();
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

    if (northWest)
    {
        northWest->queryRange(range, result);
        northEast->queryRange(range, result);
        southWest->queryRange(range, result);
        southEast->queryRange(range, result);
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

    if (northWest)
    {
        northWest->queryRange(range, callback);
        northEast->queryRange(range, callback);
        southWest->queryRange(range, callback);
        southEast->queryRange(range, callback);
    }
}

} // namespace Mino
