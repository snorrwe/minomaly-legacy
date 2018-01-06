#include "transform.h"

using namespace Mino;

std::shared_ptr<Transform> Transform::create(std::shared_ptr<Transform> parent)
{
    auto result = std::make_shared<Transform>(parent.get());
    if (parent) result->id = ++(parent->childId);
    return result;
}

Transform& Transform::addChild()
{
    children.emplace_back(this);
    auto& result = children.back();
    result.parent = this;
    result.id = ++childId;
    return result;
}

Transform& Transform::addChild(Transform& child)
{
    if (child.parent == this) return child;
    children.push_back(child);
    child.parent->removeChild(child);
    auto& result = children.back();
    result.parent = this;
    result.id = ++childId;
    return result;
}

Transform& Transform::getChild(size_t count)
{
    if (count > children.size())
    {
        throw std::runtime_error(
            "Attempting to get child that is out of bounds of this transform's children");
    }
    return children[count];
}

void Transform::removeChild(Transform& child)
{
    if (child.parent != this) return;
    auto pos = std::find_if(children.begin(), children.end(),
                            [&](auto const& c) { return c.id == child.id; });
    if (pos != children.end())
    {
        children.erase(pos);
    }
}

Transform::Transform(Transform const& t)
    : parent(t.parent), position(t.position), rotation(t.rotation), children(t.children), id(t.id)
{
}

Transform::Transform(Transform&& t)
    : parent(t.parent), position(t.position), rotation(t.rotation), children(t.children), id(t.id)
{
}

Transform& Transform::operator=(Transform const& t)
{
    parent = t.parent;
    position = Vector2<>(t.position);
    rotation = t.rotation;
    children = t.children;
    id = t.id;
    return *this;
}

Transform& Transform::operator=(Transform&& t)
{
    parent = t.parent;
    position = Vector2<>(t.position);
    rotation = t.rotation;
    children = t.children;
    id = t.id;
    return *this;
}
