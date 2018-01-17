#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "transform.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Mino
{

class ICamera
{
public:
    virtual ~ICamera() {}

    virtual void setViewport(int x, int y, int width, int height) = 0;
    virtual SDL_Rect getViewpoit() = 0;
    virtual Transform::TransformRef getTransform() = 0;
};

class Camera : public ICamera
{
public:
    static std::shared_ptr<Camera> create() { return std::make_shared<Camera>(); }

    Camera() = default;
    Camera(Camera const&) = default;
    Camera(Camera&&) = default;
    ~Camera();

    Camera& operator=(Camera const&) = default;
    Camera& operator=(Camera&&) = default;

    virtual void setViewport(int x, int y, int width, int height);
    virtual SDL_Rect getViewpoit() { return viewport; }
    virtual Transform::TransformRef getTransform() { return transform; }

private:
    void updateViewport();

    Transform::TransformRef transform = nullptr;
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    SDL_Rect viewport;
};
} // namespace Mino
