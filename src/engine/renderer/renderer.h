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

class Renderer
{
public:
    friend class Core;
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    static std::shared_ptr<Renderer> create(SDL_Window* window);

    Renderer(SDL_Renderer* renderer);
    Renderer(Renderer const&) = default;
    Renderer(Renderer&&) = default;
    ~Renderer();

    Renderer& operator=(Renderer const&) = default;
    Renderer& operator=(Renderer&&) = default;

    void render(Texture const& texture, SDL_Rect* srcrect = nullptr, SDL_Rect* dstrect = nullptr);
    void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                RotationData const& rotation);
    std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                         Color const* color = nullptr);
    void setViewport(SDL_Rect* viewport);

    SDL_Renderer* getRaw() { return renderer; }

protected:
    void clear();
    void update();

private:
    SDL_Renderer* renderer;
};
