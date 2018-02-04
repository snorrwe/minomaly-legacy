#pragma once
#include "asset.h"
#include "texture.h"
#include "transform.h"

namespace Mino::SpriteAnimationData
{

struct Frame
{
    Transform::TransformData transform = {};
    float duration = 0.0;
    Texture* texture = nullptr;
};

struct Animation
{
    enum AnimationFlags
    {
        Loop = 1,
    };
    std::vector<Frame> frames = {};
    uint8_t flags = 0;
    std::function<Animation*()> next = []() { return nullptr; };
};

} // namespace Mino::SpriteAnimationData
