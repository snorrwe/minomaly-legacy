#include "__template.h"
#include "mino.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char const* argv[])
{
    try
    {
        auto engine = Mino::Core::create<Program>("__template", SCREEN_WIDTH, SCREEN_HEIGHT);
        engine->run();
        return 0;
    }
    catch (...)
    {

        std::cout << "Unexpected and Unknown error happened!" << std::endl;
        return 1;
    }
}
