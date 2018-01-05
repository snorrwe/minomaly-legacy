#include "__template.h"

using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : ILogicSystem(core)
{
    renderer = engine->getRenderer();
    input = engine->getInput();
    subs = Subscriptions{input->onKeyDown([&](auto const& e) {
        if (e.key.keysym.sym == SDLK_ESCAPE) engine->stop();
    })};
}

Program::~Program()
{
    for (auto i = subs.begin(); i != subs.end(); ++i)
    {
        i->unsubscribe();
    }
}

void Program::start()
{
    /*
      const auto colorKey = Color(0, 0xFF, 0xFF);
      const auto rectH = 205;
      const auto rectW = 265 / 4;
      images = Texture::loadSpritesheet("assets/__template/__template.png", *renderer,
                                        {
                                            getRect(0, 0, rectW, rectH),
                                            getRect(rectW, 0, rectW, rectH),
                                            getRect(rectW * 2, 0, rectW, rectH),
                                            getRect(rectW * 3, 0, rectW, rectH),
                                        },
                                        true, &colorKey);
    */
}

SDL_Rect Program::getRect(int x, int y, int w, int h)
{
    auto rect = SDL_Rect();
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}

void Program::update()
{
    if (!(++count)) engine->stop();
}

std::shared_ptr<Texture> Program::loadTexture(std::string const& name, bool flag,
                                              Color const* color)
{
    auto result = renderer->loadTexture(name, flag, color);
    return result;
}
