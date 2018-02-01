#include "camera_component.h"

using namespace Mino;

void CameraComponent::setCamera(CameraRef value)
{
    camera = std::move(value);
    camera->setTransform(transform);
}

void CameraComponent::setTransform(Transform::TransformRef const& value)
{
    Component::setTransform(value);
    if (camera) camera->setTransform(transform);
}
