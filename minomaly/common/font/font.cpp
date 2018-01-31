#include "font.h"

using namespace Mino;

std::shared_ptr<Font> Font::load(std::string const& path, int pts)
{
    auto font = TTF_OpenFont(path.c_str(), pts);
    if (font == NULL)
    {
        std::cout << "Failed to load lazy font! SDL_ttf Error:\n"
                  << TTF_GetError() << "\nPath: " << path << std::endl;
        throw std::runtime_error("Failed to load lazy font!");
    }
    return std::make_shared<Font>(font);
}
