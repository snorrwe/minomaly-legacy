#pragma once

struct Color
{
public:
    typedef uint8_t TValue;

    Color() : red(0), green(0), blue(0) {}
    Color(TValue red, TValue green, TValue blue) : red(red), green(green), blue(blue) {}

    TValue red;
    TValue green;
    TValue blue;
};
