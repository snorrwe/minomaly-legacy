#include "surface.h"

using namespace Mino;

std::shared_ptr<Surface> Surface::loadBmp(std::string const& name, SDL_Surface* screenSurface)
{
    auto media = SDL_LoadBMP(name.c_str());
    if (media == nullptr)
    {
        std::cout << "Unable to load image [" << name << "] SDL Error: \n"
                  << SDL_GetError() << std::endl;
        throw std::runtime_error("Unable to load image");
    }
    if (screenSurface)
    {
        auto optimizedMedia = SDL_ConvertSurface(media, screenSurface->format, 0x0);
        if (optimizedMedia == nullptr)
        {
            std::cout << "Unable to optimize image! SDL Error: \n" << SDL_GetError() << std::endl;
            throw std::runtime_error("Unable to optimize image");
        }
        SDL_FreeSurface(media);
        media = optimizedMedia;
    }
    return std::make_shared<Surface>(media);
}

std::shared_ptr<Surface> Surface::loadImage(std::string const& name, SDL_Surface* screenSurface)
{
    auto media = IMG_Load(name.c_str());
    if (media == nullptr)
    {
        std::cout << "Unable to load image [" << name << "] SDL Error: \n"
                  << IMG_GetError() << std::endl;
        throw std::runtime_error("Unable to load image");
    }
    if (screenSurface)
    {
        auto optimizedMedia = SDL_ConvertSurface(media, screenSurface->format, 0x0);
        if (optimizedMedia == nullptr)
        {
            std::cout << "Unable to optimize image! SDL Error: \n" << SDL_GetError() << std::endl;
            throw std::runtime_error("Unable to optimize image");
        }
        SDL_FreeSurface(media);
        media = optimizedMedia;
    }
    return std::make_shared<Surface>(media);
}
