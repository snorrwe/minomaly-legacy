#pragma once
#include "SDL.h"
#include "core.h"
#include <iostream>
#include <memory>
#include <stdexcept>

class Core;

class Window
{
public:
    friend class Core;

    static std::unique_ptr<Window> create(const char* title, int x, int y, int w, int h,
                                          Uint32 flags);

    Window(SDL_Window* window) : window(window), surface(SDL_GetWindowSurface(window)) {}
    Window(Window const&) = delete;
    Window(Window&& w) = delete;
    ~Window() { SDL_DestroyWindow(window); }

    Window& operator=(Window&& w) = delete;
    Window& operator=(Window const&) = delete;

    void fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue);
    void blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);
    void blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);

    SDL_Surface* getSurface() { return surface; }
    SDL_Window* getWindow() { return window; }

private:
    SDL_Window* window;
    SDL_Surface* surface;
};
