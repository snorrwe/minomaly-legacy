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
    void setCamera(CameraRef value);
    virtual void setTransform(Transform::TransformRef const& value);

private:
    CameraRef camera;
};
} // namespace Mino
