#include "asset_system.h"

using namespace Mino;

std::shared_ptr<AssetSystem> AssetSystem::create(IRenderSystem* renderer)
{
    return std::make_shared<AssetSystem>(renderer);
}

std::shared_ptr<Texture> AssetSystem::loadTexture(std::string const& name)
{
    if (auto cached = findAsset(name); cached) return std::static_pointer_cast<Texture>(cached);
    auto result = Texture::loadTexture(name, *renderer);
    assets[name] = result;
    return result;
}

std::shared_ptr<Texture> AssetSystem::loadText(std::string const& text, Font const& font,
                                               Color const& color)
{
    return Texture::loadText(text, *renderer, font, color);
}

typename std::shared_ptr<AssetSystem::TSpriteSheet>
AssetSystem::loadSpriteSheet(std::string const& name, std::vector<SDL_Rect> const& rects)
{
    if (auto cached = findAsset(name); cached)
        return std::static_pointer_cast<Mino::SpriteSheet>(cached);
    auto result = Texture::loadSpriteSheet(name, *renderer, rects);
    assets[name] = result;
    return result;
}

std::shared_ptr<Asset> AssetSystem::findAsset(std::string const& name)
{
    if (auto it = assets.find(name); it != assets.end())
    {
        if (auto result = it->second.lock(); result) return result;
        assets.erase(it);
    }
    return nullptr;
}

void AssetSystem::collectGarbage()
{
    auto find = [&]() {
        return std::find_if(assets.begin(), assets.end(),
                            [](auto const& pair) { return pair.second.expired(); });
    };
    auto first = find();
    while (first != assets.end())
    {
        assets.erase(first);
        first = find();
    }
}
