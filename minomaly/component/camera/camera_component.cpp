#include "camera_component.h"

using namespace Mino;

void CameraComponent::setCamera(CameraRef value)
{
    camera = value;
    camera->setTransform(gameObject->getTransform());
}
