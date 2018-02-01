#pragma once
#include "asset.h"
#include "render_system.h"
#include "texture.h"
#include <algorithm>
#include <cassert>
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
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color()) = 0;
    virtual std::shared_ptr<TSpriteSheet> loadSpriteSheet(std::string const& name,
                                                          std::vector<SDL_Rect> const& rects) = 0;
    virtual void collectGarbage() = 0;
};

class AssetSystem : public IAssetSystem
{

    AssetMap assets = {};
    IRenderSystem* renderer;

public:
    using IAssetSystem::AssetMap;
    using IAssetSystem::TSpriteSheet;

    static std::shared_ptr<AssetSystem> create(IRenderSystem* renderer);

    AssetSystem(IRenderSystem* renderer) : renderer(renderer) { assert(renderer != nullptr); }
    virtual ~AssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name);
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color());
    virtual std::shared_ptr<TSpriteSheet> loadSpriteSheet(std::string const& name,
                                                          std::vector<SDL_Rect> const& rects);

    virtual void collectGarbage();

private:
    std::shared_ptr<Asset> findAsset(std::string const& name);
};

} // namespace Mino
