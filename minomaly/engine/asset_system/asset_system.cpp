#include "asset_system.h"

using namespace Mino;

std::shared_ptr<AssetSystem> AssetSystem::create(IRenderSystem* renderer)
{
    return std::make_shared<AssetSystem>(renderer);
}

std::shared_ptr<Texture> AssetSystem::loadTexture(std::string const& name)
{
    return load<Texture>(name, [&]() { return Texture::loadTexture(name, *renderer); });
}

std::shared_ptr<Texture> AssetSystem::loadText(std::string const& text, Font const& font,
                                               Color const& color)
{
    return Texture::loadText(text, *renderer, font, color);
}

typename std::shared_ptr<AssetSystem::TSpriteSheet>
AssetSystem::loadSpriteSheet(std::string const& name, std::vector<SDL_Rect> const& rects)
{
    return load<TSpriteSheet>(name,
                              [&]() { return Texture::loadSpriteSheet(name, *renderer, rects); });
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

std::shared_ptr<Font> AssetSystem::loadFont(std::string const& path, int pts)
{
    return load<Font>(path, [&]() { return Font::load(path, pts); });
}

std::shared_ptr<Audio> AssetSystem::loadWAV(std::string const& path)
{
    return load<Audio>(path, [&]() { return Audio::loadWAV(path); });
}

std::shared_ptr<Music> AssetSystem ::loadMusic(std::string const& path)
{
    return load<Music>(path, [&]() { return Music::loadMusic(path); });
}
