#pragma once
#include "SDL.h"
#include "core.h"
#include "vector2.h"
#include <iostream>
#include <memory>
#include <stdexcept>

namespace Mino
{
class EngineCore;

class IWindowSystem
{
public:
    virtual ~IWindowSystem() {}

    virtual void fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) = 0;
    virtual void blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) = 0;

    virtual Vector2<int> size() = 0;
};

class WindowSystem : public IWindowSystem
{
public:
    friend class EngineCore;

    static std::unique_ptr<WindowSystem>
    create(const char* title, int x, int y, int w, int h, Uint32 flags);

    WindowSystem(SDL_Window* window, int width, int height)
        : window(window)
        , surface(SDL_GetWindowSurface(window))
        , width(width)
        , height(height)
    {
    }
    WindowSystem(WindowSystem const&) = delete;
    WindowSystem(WindowSystem&& w) = delete;
    virtual ~WindowSystem() { SDL_DestroyWindow(window); }

    WindowSystem& operator=(WindowSystem&& w) = delete;
    WindowSystem& operator=(WindowSystem const&) = delete;

    virtual void fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue);
    virtual void blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);
    virtual void blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);

    operator SDL_Window*() { return window; }
    operator SDL_Surface*() { return surface; }

    virtual Vector2<int> size() { return {width, height}; }

private:
    SDL_Window* window;
    SDL_Surface* surface;
    int width;
    int height;
};

} // namespace Mino
