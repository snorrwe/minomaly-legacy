#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

class Surface
{
public:
    static std::shared_ptr<Surface> loadBmp(std::string const& name,
                                          SDL_Surface* screenSurface = nullptr);
    static std::shared_ptr<Surface> loadImage(std::string const& name,
                                            SDL_Surface* screenSurface = nullptr);

    Surface(SDL_Surface* media) : media(media) {}
    ~Surface() { SDL_FreeSurface(media); }

    SDL_Surface* get() { return media; }

private:
    SDL_Surface* media;
};
