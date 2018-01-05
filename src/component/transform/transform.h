#pragma once
#include "renderdata.h"
#include "vector2.h"

namespace Mino
{

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

private:
    Vector2<> position;
    Mino::RenderData::RotationData rotation;
};

} // namespace Mino
