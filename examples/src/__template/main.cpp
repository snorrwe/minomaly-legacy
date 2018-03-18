#include "mino.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace Mino;

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

class Program : public Mino::Application
{
    using RotationData = Mino::RenderData::RotationData;
    using Subscriptions = std::array<Mino::ISubscription, 1>;
    using MediaContainer = std::shared_ptr<Mino::SpriteSheet>;

    Mino::IInputSystem* input;
    Subscriptions subs;
    MediaContainer images;

public:
    virtual ~Program()
    {
        for (auto i = subs.begin(); i != subs.end(); ++i)
        {
            i->unsubscribe();
        }
    }

    void start()
    {
        input = getEngineCore()->getInput();
        subs = Subscriptions{input->onKeyDown([&](auto const& e) {
            if (e.key.keysym.sym == SDLK_ESCAPE) getEngineCore()->stop();
        })};
    }
};

int main(int argc, char const* argv[])
{
    try
    {
        auto engine = Mino::EngineCore::create<Program>("__template", SCREEN_WIDTH, SCREEN_HEIGHT);
        engine->run();
        return 0;
    }
    catch (...)
    {
        std::cout << "Unexpected and Unknown error happened!" << std::endl;
        return 1;
    }
}
