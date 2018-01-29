#pragma once
#include "render_system.h"
#include "texture.h"
#include <cassert>
#include <memory>

namespace Mino
{

class IAssetSystem
{
public:
    using TSpriteSheet = Mino::Texture::TSpriteSheet;

    virtual ~IAssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const& colorKey = Color()) = 0;
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color()) = 0;
    virtual TSpriteSheet loadSpritesheet(std::string const& name,
                                         std::vector<SDL_Rect> const& rects, bool flag = false,
                                         Color const& colorKey = Color()) = 0;
};

class AssetSystem : public IAssetSystem
{
    IRenderSystem* renderer;

public:
    AssetSystem(IRenderSystem* renderer) : renderer(renderer) { assert(renderer != nullptr); }
    virtual ~AssetSystem() {}

    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const& colorKey = Color());
    virtual std::shared_ptr<Texture> loadText(std::string const& text, Font const& font,
                                              Color const& color = Color());
    virtual TSpriteSheet loadSpritesheet(std::string const& name,
                                         std::vector<SDL_Rect> const& rects, bool flag = false,
                                         Color const& colorKey = Color());
};

} // namespace Mino
