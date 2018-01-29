#include "asset_system.h"

using namespace Mino;

std::shared_ptr<Texture> AssetSystem::loadTexture(std::string const& name, bool flag,
                                                  Color const& colorKey)
{
    return Texture::loadTexture(name, *renderer, flag, colorKey);
}

std::shared_ptr<Texture> AssetSystem::loadText(std::string const& text, Font const& font,
                                               Color const& color)
{
    return Texture::loadText(text, *renderer, font, color);
}

typename AssetSystem::TSpriteSheet AssetSystem::loadSpritesheet(std::string const& name,
                                                                std::vector<SDL_Rect> const& rects,
                                                                bool flag, Color const& colorKey)
{
    return Texture::loadSpritesheet(name, *renderer, rects, flag, colorKey);
}
