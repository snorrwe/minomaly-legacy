#include "camera.h"

using namespace Mino;

Camera::~Camera() {}

void Camera::updateViewport()
{
    viewport.x = x;
    viewport.y = y;
    viewport.w = width;
    viewport.h = height;
}

void Camera::setViewport(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    updateViewport();
}

Vector2<double> Camera::screenToWorldPosition(Vector2<int> const& screenPoint)
{
    if (!transform) return {(double)screenPoint.x(), (double)screenPoint.y()};
    auto& pos = transform->getPosition();
    return {screenPoint.x() - pos.x(), -screenPoint.y() + pos.y()};
}
