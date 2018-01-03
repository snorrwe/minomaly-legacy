#include "spritesheet.h"

using namespace Spritesheet;

Program::Program(std::shared_ptr<Core> core) : ILogic(core)
{
    window = engine->getWindow();
    renderer = engine->getRenderer();
    camera = Camera::create(renderer);
    const auto padding = 15;
    camera->updateViewport(padding, padding, SCREEN_WIDTH - padding * 2,
                           SCREEN_HEIGHT - padding * 2);
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

void Program::start() { loadMedia(); }

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
    images[0]->render({0, 0});
    images[1]->render({SCREEN_WIDTH - images[1]->getSrcrect()->w, 0});
    images[2]->render({0, SCREEN_HEIGHT - images[2]->getSrcrect()->h});
    images[3]->render(
        {SCREEN_WIDTH - images[3]->getSrcrect()->w, SCREEN_HEIGHT - images[3]->getSrcrect()->h});
}

void Program::loadMedia()
{
    const auto colorKey = Color(0, 0xFF, 0xFF);
    images = Texture::loadSpritesheet("assets/spritesheet/dots.png", *renderer,
                                      {getRect(0, 0, 100, 100), getRect(100, 0, 100, 100),
                                       getRect(0, 100, 100, 100), getRect(100, 100, 100, 100)},
                                      true, &colorKey);
}

std::shared_ptr<Texture> Program::loadTexture(std::string const& name, bool flag,
                                              Color const* color)
{
    auto result = renderer->loadTexture(name, flag, color);
    return result;
}
