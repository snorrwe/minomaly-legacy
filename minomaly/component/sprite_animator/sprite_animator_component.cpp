#include "sprite_animator_component.h"

using namespace Mino;

void SpriteAnimatorComponent::start()
{
    time = Services::get<ITimeService>().get();
    renderer = gameObject->getComponent<SpriteRendererComponent>();
    transform = gameObject->getTransform();
    assert(time);
    assert(renderer);
}

void SpriteAnimatorComponent::update()
{
    if (!current) return;

    auto& frame = current->frames[currentFrame];

    currentRunTime += time->deltaTime();
    if (currentRunTime < frame.duration) return;

    currentRunTime = 0.0f;
    ++currentFrame;
    if (currentFrame >= current->frames.size())
    {
        if (current->flags & Animation::AnimationFlags::Loop)
        {
            reset();
        }
        else if (auto nextAnimation = current->next(); nextAnimation)
        {
            startAnimation(*nextAnimation);
            return;
        }
        else
        {
            reset();
        }
    }
    applyFrame(current->frames[currentFrame]);
}

void SpriteAnimatorComponent::startAnimation(Animation& animation)
{
    current = &animation;
    currentFrame = 0;
    currentRunTime = 0.0f;
    applyFrame(animation.frames[0]);
}

void SpriteAnimatorComponent::applyFrame(Frame& frame)
{ /*TODO: transform*/
    renderer->setTexture(frame.texture);
}

void SpriteAnimatorComponent::reset() { currentFrame = 0; }
