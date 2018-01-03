#include "SDL.h"
#undef main
#include "alpha.h"
#include "core.h"
#include <iostream>
#include <stdexcept>

using Spritesheet::Program;
using Spritesheet::SCREEN_HEIGHT;
using Spritesheet::SCREEN_WIDTH;

int main(int argc, char const* argv[])
{
    try
    {
        auto engine = Core::create<Program>("SDL test", SCREEN_WIDTH, SCREEN_HEIGHT);
        engine->run();
        return 0;
    }
    catch (std::exception const& e)
    {
        std::cout << "Unexpected error happened!\n" << e.what() << std::endl;
        return 1;
    }
}
