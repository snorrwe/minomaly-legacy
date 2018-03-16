#pragma once
#include "asset.h"
#include "audio.h"
#include "font.h"
#include "render_system.h"
#include "texture.h"
#include <algorithm>
#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Mino
{
class Texture;
struct SpriteSheet;

class IAssetSystem
{
public:
    using TSpriteSheet = SpriteSheet;
    using AssetMap = std::map<std::string, std::weak_ptr<Asset>>;

    virtual ~IAssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name) = 0;
    virtual std::shared_ptr<Texture>
    loadText(std::string const& text, Font const& font, Color const& color = Color()) = 0;
    virtual std::shared_ptr<TSpriteSheet> loadSpriteSheet(std::string const& name,
                                                          std::vector<SDL_Rect> const& rects)
        = 0;
    virtual std::shared_ptr<Font> loadFont(std::string const& path, int pts) = 0;
    virtual std::shared_ptr<Audio> loadWAV(std::string const& path) = 0;
    virtual std::shared_ptr<Music> loadMusic(std::string const& path) = 0;

    virtual void collectGarbage() = 0;
    virtual void clear() = 0;
};

class AssetSystem : public IAssetSystem
{
    AssetMap assets = {};
    IRenderSystem* renderer;

public:
    using IAssetSystem::AssetMap;
    using IAssetSystem::TSpriteSheet;

    static std::unique_ptr<AssetSystem> create(IRenderSystem* renderer);

    AssetSystem(IRenderSystem* renderer)
        : renderer(renderer)
    {
        assert(renderer != nullptr);
    }
    virtual ~AssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name);
    virtual std::shared_ptr<Texture>
    loadText(std::string const& text, Font const& font, Color const& color = Color());
    virtual std::shared_ptr<TSpriteSheet> loadSpriteSheet(std::string const& name,
                                                          std::vector<SDL_Rect> const& rects);
    virtual std::shared_ptr<Font> loadFont(std::string const& path, int pts);
    virtual std::shared_ptr<Audio> loadWAV(std::string const& path);
    virtual std::shared_ptr<Music> loadMusic(std::string const& path);

    virtual void collectGarbage();
    virtual void clear() { assets.clear(); }

private:
    std::shared_ptr<Asset> findAsset(std::string const& name);
    template <typename T>
    std::shared_ptr<T> load(std::string const& key, std::function<std::shared_ptr<T>()>);
};

template <typename T>
std::shared_ptr<T> AssetSystem::load(std::string const& key,
                                     std::function<std::shared_ptr<T>()> loadFn)
{
    if (auto cached = findAsset(key); cached) return std::static_pointer_cast<T>(cached);
    auto result = loadFn();
    assets[key] = result;
    return result;
}

} // namespace Mino
