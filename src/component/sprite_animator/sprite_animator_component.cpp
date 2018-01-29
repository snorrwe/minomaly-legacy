#include "sprite_animator_component.h"

using namespace Mino;

void SpriteAnimatorComponent::start()
{
    time = Services::get<ITimeService>().get();
    renderer = gameObject->getComponent<SpriteRendererComponent>();
    assert(time);
    assert(renderer);
}

void SpriteAnimatorComponent::update()
{
    if (!current) return;

    auto& frame = current->frames[currentFrame];

    currentRunTime += time->deltaTime();
    if (currentRunTime < frame.duration) return;

    auto nextFrame = currentFrame + 1;
    if (nextFrame < current->frames.size())
    {
        applyFrame(current->frames[nextFrame]);
        return;
    }

    if (current->flags | Animation::AnimationFlags::Loop)
    {
        currentFrame = 0;
        currentRunTime = 0.0f;
    }
    else
    {
        auto nextAnimation = current->next();
        startAnimation(*nextAnimation);
    }
}

void SpriteAnimatorComponent::startAnimation(Animation& animation)
{
    current = &animation;
    currentFrame = 0;
    currentRunTime = 0.0f;
}

void SpriteAnimatorComponent::applyFrame(Frame& frame)
{ /*TODO: transform*/
    renderer->setTexture(frame.texture);
}
