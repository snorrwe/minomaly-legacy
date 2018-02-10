#pragma once
#include "audio_system.h"
#include "component.h"
#include "core.h"
#include <cassert>

namespace Mino
{
class AudioPlayerComponent : public Component
{
public:
    virtual void start();

protected:
    IAudioSystem* audioSystem = nullptr;
};

} // namespace Mino
