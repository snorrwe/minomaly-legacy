#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "asset.h"
#include "camera.h"
#include "color.h"
#include "font.h"
#include "p_managed_texture.h"
#include "render_system.h"
#include "renderdata.h"
#include "surface.h"
#include "transform.h"
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace Mino
{

class IRenderSystem;
class ICamera;
class Texture;

struct SpriteSheet : public Asset, std::vector<std::shared_ptr<Texture>>
{
};

class Texture : public Asset
{
public:
    using TSpriteSheet = std::shared_ptr<SpriteSheet>;
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;
    using ManagedTexture = Private::ManagedTexture;

    static std::shared_ptr<Texture> loadTexture(std::string const& name, IRenderSystem& renderer,
                                                bool flag = false, Color const& colorKey = Color());
    static std::shared_ptr<Texture> loadText(std::string const& text, IRenderSystem& renderer,
                                             Font const& font, Color const& color = Color());
    static TSpriteSheet loadSpriteSheet(std::string const& name, IRenderSystem& renderer,
                                        std::vector<SDL_Rect> const& rects, bool flag = false,
                                        Color const& colorKey = Color());
    static std::shared_ptr<Texture> fromSurface(std::shared_ptr<Surface> surface,
                                                IRenderSystem& renderer);

    Texture(std::shared_ptr<ManagedTexture> texture, int width, int height,
            IRenderSystem& renderer);
    Texture(Texture const& t) = default;
    Texture(Texture&&) = default;
    ~Texture() {}

    Texture& operator=(Texture const&) = default;
    Texture& operator=(Texture&&) = default;

    std::shared_ptr<ManagedTexture> get() const { return texture; }
    SDL_Texture* getRaw() const { return texture->texture; }
    SDL_Rect* getSrcrect() const { return srcrect.get(); }
    void setSrcrect(std::shared_ptr<SDL_Rect> rect);
    int getWidth() { return width; }
    int getHeight() { return height; }

    void render(Vector2 const& pos) const;
    void render(Transform::TransformData const&) const;
    void render(Vector2 const& pos,
                std::function<void(Texture const&, SDL_Rect*, SDL_Rect*)>) const;
    void color(Color const& color);
    void setBlendMode(SDL_BlendMode blending);
    void setAlpha(uint8_t alpha);

private:
    std::shared_ptr<SDL_Rect> srcrect = std::shared_ptr<SDL_Rect>(nullptr);
    std::shared_ptr<ManagedTexture> texture;
    int width;
    int height;
    IRenderSystem& renderer;
};

} // namespace Mino
