#pragma once
#include "SDL.h"
#include "color.h"
#include "core.h"
#include "renderdata.h"
#include "texture.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Core;
class Texture;

class IRenderer
{
public:
    friend class Core;

    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    virtual ~IRenderer() {}

    virtual void render(Texture const& texture, SDL_Rect* srcrect = nullptr,
                        SDL_Rect* dstrect = nullptr) = 0;
    virtual void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                        RotationData const& rotation) = 0;
    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const* color = nullptr) = 0;
    virtual void setViewport(SDL_Rect* viewport) = 0;
    virtual void clear() = 0;
    virtual void update() = 0;

    virtual SDL_Renderer* getRaw() = 0;
};

class Renderer : public IRenderer
{
public:
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    static std::shared_ptr<IRenderer> create(SDL_Window* window);

    Renderer(SDL_Renderer* renderer);
    Renderer(Renderer const&) = default;
    Renderer(Renderer&&) = default;
    virtual ~Renderer();

    Renderer& operator=(Renderer const&) = default;
    Renderer& operator=(Renderer&&) = default;

    virtual void render(Texture const& texture, SDL_Rect* srcrect = nullptr,
                        SDL_Rect* dstrect = nullptr);
    virtual void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                        RotationData const& rotation);
    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const* color = nullptr);
    virtual void setViewport(SDL_Rect* viewport);
    virtual void clear();
    virtual void update();

    virtual SDL_Renderer* getRaw() { return renderer; }

private:
    SDL_Renderer* renderer;
};
