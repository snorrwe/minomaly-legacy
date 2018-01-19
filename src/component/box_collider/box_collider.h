#pragma once
#include "collider.h"
#include "vector2.h"
#include <algorithm>
#include <iostream>
#include <vector>

namespace Mino
{

class BoxColliderComponent : public ColliderComponent
{
public:
    enum Corner
    {
        TopLeft = 0,
        TopRight = 1,
        BottomLeft = 2,
        BottomRight = 3
    };

    virtual void start();

    float getWidth() const { return width; }
    float getHeight() const { return height; }
    void set(float w, float h, Vector2<float> offset = {0, 0});

    virtual void checkCollisions();
    virtual BoundingBox asBoundingBox() const;

protected:
    void removeSelf(std::vector<World::Node>&);

    Vector2<float> offset = {0, 0};
    float width = 0;
    float height = 0;
};

} // namespace Mino
