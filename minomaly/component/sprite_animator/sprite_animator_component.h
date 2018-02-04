#pragma once
#include "component.h"
#include "services.h"
#include "sprite_animation.h"
#include "sprite_renderer.h"
#include "texture.h"
#include "transform.h"
#include <cassert>
#include <functional>
#include <vector>

namespace Mino
{

class SpriteAnimatorComponent : public Component
{
    using Animation = SpriteAnimationData::Animation;
    using Frame = SpriteAnimationData::Frame;

    ITimeService* time;
    Animation* current;
    SpriteRendererComponent* renderer;
    float currentRunTime = 0.0f;
    size_t currentFrame = 0;
    Transform::TransformRef transform;

public:
    virtual void start();
    virtual void update();
    void startAnimation(Animation&);

private:
    void applyFrame(Frame& frame);
    void reset();
};

} // namespace Mino
