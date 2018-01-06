#include "bloke.h"

using namespace BlokeGame;
using namespace Mino;

Program::Program(std::shared_ptr<Core> core) : Scene(core)
{

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

void Program::update()
{
    handleKey(SDLK_UP, 0, -1);
    handleKey(SDLK_DOWN, 0, 1);
    handleKey(SDLK_LEFT, -1, 0);
    handleKey(SDLK_RIGHT, 1, 0);
    if (!(++delay % 10)) // Update position in every 10th update
    {
        x += vx;
        y += vy;
    }
    images[0]->render({0, 0}); // background
    images[1]->render({x, y}, *camera);
}

void Program::handleKey(SDL_Keycode const& code, int vx, int vy)
{
    if (input->isDown(code))
    {
        this->vx = vx;
        this->vy = vy;
    }
    else
    {
        if (this->vx == vx) this->vx = 0;
        if (this->vy == vy) this->vy = 0;
    }
}

void Program::loadMedia()
{
    const auto color = Color(0, 0xFF, 0xFF);
    images = MediaContainer{
        loadTexture("assets/bloke/background.png"),
        loadTexture("assets/bloke/bloke.png", true, &color),
    };
}

std::shared_ptr<Texture> Program::loadTexture(std::string const& name, bool flag,
                                              Color const* color)
{
    auto result = renderer->loadTexture(name, flag, color);
    return result;
}
