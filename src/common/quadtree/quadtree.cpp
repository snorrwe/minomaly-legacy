#include "quadtree.h"

using namespace Mino;

bool Quadtree::insert(typename Quadtree::Vector const& v)
{
    if (!boundary.containsPoint(v)) return false;

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

void Quadtree::subdivide()
{
    auto subDimension = boundary.getHalfDimension() * 0.5;
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

bool Quadtree::contains(Vector const& v)
{
    return query(v, [&](Quadtree& qt) {
        return std::find(qt.points.begin(), qt.points.end(), v) != qt.points.end();
    });
}

bool Quadtree::propagateUp(Vector const& v)
{
    if (boundary.containsPoint(v)) return insert(v);
    if (!parent) return false;
    return parent->propagateUp(v);
}

bool Quadtree::move(Vector const& from, Vector const& to)
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

bool Quadtree::erase(Vector const& v)
{
    return query(v, [&](Quadtree& qt) {
        auto i = std::find(qt.points.begin(), qt.points.end(), v);
        auto result = i != qt.points.end();
        if (result) qt.points.erase(i);
        return result;
    });
}

bool Quadtree::query(Vector const& v, std::function<bool(Quadtree&)> callback)
{
    if (callback(*this)) return true;
    if (!northWest) return false;
    auto center = boundary.getCenter();
    if (v.x() < center.x())
    {
        if (v.y() > center.y())
            return northWest->query(v, callback);
        else if (v.y() < center.y())
            return southWest->query(v, callback);
        return northWest->query(v, callback) || southWest->query(v, callback);
    }
    else if (v.x() > center.x())
    {
        if (v.y() > center.y())
            return northEast->query(v, callback);
        else if (v.y() < center.y())
            return southEast->query(v, callback);
        return northEast->query(v, callback) || southEast->query(v, callback);
    }
    // v.x == center.x ==> could be on either edge
    if (v.y() < center.y())
        return southWest->query(v, callback) || southEast->query(v, callback);
    else if (v.y() > center.y())
        return northWest->query(v, callback) || northEast->query(v, callback);
    // v == center ==> it could be in either of these
    return (northWest->query(v, callback) || northEast->query(v, callback) ||
            southWest->query(v, callback) || southEast->query(v, callback));
}
