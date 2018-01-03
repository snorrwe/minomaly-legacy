#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "renderdata.h"
#include "renderer.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

class Renderer;
class Texture;

class Camera
{
public:
    using RotationData = RenderData::RotationData;

    static std::shared_ptr<Camera> create(std::shared_ptr<Renderer> renderer)
    {
        return std::make_shared<Camera>(renderer);
    }

    Camera(std::shared_ptr<Renderer> renderer);
    Camera(Camera const&) = default;
    Camera(Camera&&) = default;
    ~Camera();

    Camera& operator=(Camera const&) = default;
    Camera& operator=(Camera&&) = default;

    void updateViewport(int x, int y, int width, int height);
    void render(Texture const& texture, SDL_Rect* srcrect = nullptr, SDL_Rect* dstrect = nullptr);
    void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                RotationData const& rotation);

    std::shared_ptr<Renderer> getRenderer() { return renderer; }

private:
    void updateViewport();

    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    SDL_Rect viewport;
    std::shared_ptr<Renderer> renderer;
};
