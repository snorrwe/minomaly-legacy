#pragma once
#include "render_system.h"
#include "texture.h"
#include <cassert>
#include <memory>
#include <vector>

namespace Mino
{
class Texture;

class IAssetSystem
{
public:
    using TSpriteSheet = std::vector<std::shared_ptr<Texture>>;

    virtual ~IAssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name) = 0;
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color()) = 0;
    virtual TSpriteSheet loadSpriteSheet(std::string const& name,
                                         std::vector<SDL_Rect> const& rects) = 0;
};

class AssetSystem : public IAssetSystem
{
    IRenderSystem* renderer;

public:
    using IAssetSystem::TSpriteSheet;

    static std::shared_ptr<AssetSystem> create(IRenderSystem* renderer);

    AssetSystem(IRenderSystem* renderer) : renderer(renderer) { assert(renderer != nullptr); }
    virtual ~AssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name);
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color());
    virtual TSpriteSheet loadSpriteSheet(std::string const& name,
                                         std::vector<SDL_Rect> const& rects);
};

} // namespace Mino
