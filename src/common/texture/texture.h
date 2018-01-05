#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "camera.h"
#include "color.h"
#include "renderdata.h"
#include "renderer.h"
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class IRenderSystem;
class ICamera;

struct ManagedTexture
{
    ManagedTexture(SDL_Texture* texture) : texture(texture) {}
    ManagedTexture(ManagedTexture const&) = delete;
    ManagedTexture(ManagedTexture&&) = delete;
    ~ManagedTexture() { SDL_DestroyTexture(texture); }

    ManagedTexture& operator=(ManagedTexture const&) = delete;
    ManagedTexture& operator=(ManagedTexture&&) = delete;

    operator SDL_Texture*() { return texture; }

    SDL_Texture* texture;
};

class Texture
{
public:
    using TSpriteSheet = std::vector<std::shared_ptr<Texture>>;
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    static std::shared_ptr<Texture> loadTexture(std::string const& name, IRenderSystem& renderer,
                                                bool flag = false, Color const* colorKey = nullptr);
    static TSpriteSheet loadSpritesheet(std::string const& name, IRenderSystem& renderer,
                                        std::vector<SDL_Rect> const& rects, bool flag = false,
                                        Color const* colorKey = nullptr);

    Texture(std::shared_ptr<ManagedTexture> texture, int width, int height, IRenderSystem& renderer);
    Texture(Texture const& t) = default;
    Texture(Texture&&) = default;
    ~Texture() {}

    Texture& operator=(Texture const&) = default;
    Texture& operator=(Texture&&) = default;

    std::shared_ptr<ManagedTexture> get() const { return texture; }
    SDL_Texture* getRaw() const { return texture->texture; }
    SDL_Rect* getSrcrect() const { return srcrect.get(); }
    void setSrcrect(std::shared_ptr<SDL_Rect> rect);

    void render(Vector2 const& pos) const;
    void render(Vector2 const& pos, ICamera& camera) const;
    void render(Vector2 const& pos, RotationData const& rotation) const;
    void render(Vector2 const& pos, ICamera& camera, RotationData const& rotation) const;
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
