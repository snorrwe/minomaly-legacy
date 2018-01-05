#include "camera.h"

Camera::Camera(std::shared_ptr<IRenderSystem> renderer) : viewport(), renderer(renderer)
{
    updateViewport();
}

Camera::~Camera() {}

void Camera::updateViewport()
{
    viewport.x = x;
    viewport.y = y;
    viewport.w = width;
    viewport.h = height;
}

void Camera::updateViewport(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    updateViewport();
}

void Camera::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    renderer->setViewport(&viewport);
    renderer->render(texture, srcrect, dstrect);
}

void Camera::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                    RotationData const& rotation)
{
    renderer->setViewport(&viewport);
    renderer->render(texture, srcrect, dstrect, rotation);
}
