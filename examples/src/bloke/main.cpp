#include "bloke.h"
#include "mino.h"
#include <iostream>
#include <stdexcept>

using BlokeGame::Program;
using BlokeGame::SCREEN_HEIGHT;
using BlokeGame::SCREEN_WIDTH;

int main(int argc, char const* argv[])
{
    try
    {
        auto engine = Mino::Core::create<Program>("SDL test", SCREEN_WIDTH, SCREEN_HEIGHT);
        engine->run();
        return 0;
    }
    catch (std::exception const& e)
    {
        std::cout << "Unexpected error happened!\n" << e.what() << std::endl;
        return 1;
    }
}
