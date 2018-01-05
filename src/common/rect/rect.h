#pragma once
#include "SDL.h"

namespace Mino
{

class Rectangle
{
public:
    Rectangle() = default;
    Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    Rectangle(Rectangle const&) = default;
    Rectangle(Rectangle&&) = default;
    ~Rectangle() = default;

    operator SDL_Rect()
    {
        auto rect = SDL_Rect();
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        return rect;
    }

    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

} // namespace Mino
