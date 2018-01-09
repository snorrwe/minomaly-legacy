#include "mino.h"
#include "mouse.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char const* argv[])
{
    try
    {
        auto engine = Mino::EngineCore::create<Program>("mouse", SCREEN_WIDTH, SCREEN_HEIGHT);
        engine->run();
        return 0;
    }
    catch (std::exception const& e)
    {
        std::cout << "Unexpected error happened!\n" << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {

        std::cout << "Unexpected and Unknown error happened!" << std::endl;
        return 2;
    }
}
