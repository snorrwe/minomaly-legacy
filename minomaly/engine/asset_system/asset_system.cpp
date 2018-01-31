#include "asset_system.h"

using namespace Mino;

std::shared_ptr<AssetSystem> AssetSystem::create(IRenderSystem* renderer)
{
    return std::make_shared<AssetSystem>(renderer);
}

std::shared_ptr<Texture> AssetSystem::loadTexture(std::string const& name)
{
    return Texture::loadTexture(name, *renderer);
}

std::shared_ptr<Texture> AssetSystem::loadText(std::string const& text, Font const& font,
                                               Color const& color)
{
    return Texture::loadText(text, *renderer, font, color);
}

typename AssetSystem::TSpriteSheet AssetSystem::loadSpriteSheet(std::string const& name,
                                                                std::vector<SDL_Rect> const& rects)
{
    return Texture::loadSpriteSheet(name, *renderer, rects);
}
