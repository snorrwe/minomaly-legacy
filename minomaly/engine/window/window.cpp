#include "window.h"

using namespace Mino;

void WindowSystem::fillRect(const SDL_Rect* rect, uint8_t red, uint8_t green, uint8_t blue)
{
    auto color = SDL_MapRGB(surface->format, red, green, blue);
    SDL_FillRect(surface, rect, color);
}

std::unique_ptr<WindowSystem>
WindowSystem::create(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    auto window = SDL_CreateWindow(title, x, y, w, h, flags);
    if (window == nullptr)
    {
        auto error = SDL_GetError();
        std::cout << "Window could not be created! SDL_Error: \n" << error << std::endl;
        throw std::runtime_error(error);
    }
    return std::make_unique<WindowSystem>(window, w, h);
}

void WindowSystem::blitSurface(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    SDL_BlitSurface(src, srcrect, surface, dstrect);
}

void WindowSystem::blitScaled(SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    SDL_BlitScaled(src, srcrect, surface, dstrect);
}
