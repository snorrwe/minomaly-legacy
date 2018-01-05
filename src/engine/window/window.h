#pragma once
#include "SDL.h"
#include "core.h"
#include <iostream>
#include <memory>
#include <stdexcept>

namespace Mino
{

class Core;

class IWindow
{
public:
    virtual ~IWindow() {}

    virtual void fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue) = 0;
    virtual void blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) = 0;
    virtual void blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect) = 0;
};

class Window : public IWindow
{
public:
    friend class Core;

    static std::unique_ptr<Window> create(const char* title, int x, int y, int w, int h,
                                          Uint32 flags);

    Window(SDL_Window* window) : window(window), surface(SDL_GetWindowSurface(window)) {}
    Window(Window const&) = delete;
    Window(Window&& w) = delete;
    virtual ~Window() { SDL_DestroyWindow(window); }

    Window& operator=(Window&& w) = delete;
    Window& operator=(Window const&) = delete;

    virtual void fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue);
    virtual void blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);
    virtual void blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);

    SDL_Surface* getSurface() { return surface; }
    SDL_Window* getWindow() { return window; }

private:
    SDL_Window* window;
    SDL_Surface* surface;
};

} // namespace Mino
