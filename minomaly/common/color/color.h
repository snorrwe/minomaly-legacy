#pragma once

namespace Mino
{

struct Color
{
public:
    typedef uint8_t TValue;

    Color() = default;
    Color(TValue red, TValue green, TValue blue, TValue alpha = 1)
        : red(red), green(green), blue(blue), alpha(alpha)
    {
    }
    ~Color() = default;

    operator SDL_Color() const { return {red, green, blue, alpha}; }

    TValue red = 0;
    TValue green = 0;
    TValue blue = 0;
    TValue alpha = 1;
};

} // namespace Mino
