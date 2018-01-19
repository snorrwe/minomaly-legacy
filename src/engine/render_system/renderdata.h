#pragma once
#include "SDL.h"

namespace Mino::RenderData
{
struct Vector2
{
    Vector2() = default;
    Vector2(int x, int y) : x(x), y(y) {}
    Vector2(Vector2 const&) = default;
    Vector2(Vector2&&) = default;

    Vector2& operator=(Vector2 const&) = default;
    Vector2& operator=(Vector2&&) = default;

    operator SDL_Point() const
    {
        auto result = SDL_Point();
        result.x = x;
        result.y = y;
        return result;
    }

    int x = 0;
    int y = 0;
};

struct RotationData
{
    RotationData() = default;
    RotationData(float angle, SDL_RendererFlip flip, Vector2 center)
        : angle(angle), flip(flip), center(center)
    {
    }
    RotationData(RotationData const&) = default;
    RotationData(RotationData&&) = default;

    RotationData& operator=(RotationData const&) = default;
    RotationData& operator=(RotationData&&) = default;

    float angle = 0.0;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    Vector2 center = Vector2();
};
} // namespace Mino::RenderData
