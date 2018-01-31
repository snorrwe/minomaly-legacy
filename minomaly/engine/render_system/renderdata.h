#pragma once
#include "SDL.h"
#include "vector2.h"

namespace Mino::RenderData
{
using Vector2 = Vector2<int>;

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
    Vector2 center = {};

    RotationData operator+(RotationData const& rd) const
    {
        auto result = RotationData{};
        result.angle = angle + rd.angle;
        result.center = center + rd.center;
        result.flip = static_cast<SDL_RendererFlip>(flip | rd.flip);
        return result;
    }

    RotationData operator-(RotationData const& rd) const
    {
        auto result = RotationData{};
        result.angle = angle - rd.angle;
        result.center = center - rd.center;
        result.flip = static_cast<SDL_RendererFlip>(flip | rd.flip);
        return result;
    }
};

} // namespace Mino::RenderData
