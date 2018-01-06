#pragma once
#include "component.h"

namespace Mino
{

class SpriteRendererComponent : public Component
{
public:
    virtual void start() { gameObject->getTransform(); }

protected:
    Transform* transform;
};

} // namespace Mino
