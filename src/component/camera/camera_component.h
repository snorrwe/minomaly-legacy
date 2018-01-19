#pragma once
#include "camera.h"
#include "component.h"
#include <memory>

namespace Mino
{

class CameraComponent : public Component
{
public:
    using CameraRef = Camera::CameraReference;

    CameraRef getCamera() { return camera; }
    void setCamera(CameraRef value)
    {
        camera = value;
        camera->setTransform(gameObject->getTransform());
    }

private:
    CameraRef camera;
};
} // namespace Mino
