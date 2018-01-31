#pragma once
#include "component.h"
#include "services.h"
#include "sprite_renderer.h"
#include "texture.h"
#include "transform.h"
#include <cassert>
#include <functional>
#include <vector>

namespace Mino
{

namespace SpriteAnimationData
{

struct Frame
{
    Transform::TransformData transform = {};
    float duration = 0.0;
    Texture* texture = nullptr;
};

struct Animation
{
    enum AnimationFlags
    {
        Loop = 1,
    };
    std::vector<Frame> frames = {};
    uint8_t flags = 0;
    std::function<Animation*()> next = []() { return nullptr; };
};

} // namespace SpriteAnimationData

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
