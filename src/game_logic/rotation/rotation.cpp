#include "rotation.h"

using namespace Rotation;

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

void Program::start()
{
    loadMedia();
    rotation.center.x = dudeWidth / 2;
    rotation.center.y = dudeHeight / 2;
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
    if (!(++delay))
    {
        frame = (++frame) % 4;
        rotation.angle += direction;
        rotationDelay = (rotationDelay + abs(direction)) % 360;
    }
    if (!rotationDelay)
    {
        rotationDelay = (rotationDelay + abs(direction)) % 360;
        direction *= -1;
        if (direction < 0)
        {
            rotation.flip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            rotation.flip = SDL_FLIP_NONE;
        }
    }
    images[frame]->render({x, y}, rotation);
}

void Program::loadMedia()
{
    const auto colorKey = Color(0, 0xFF, 0xFF);
    images = Texture::loadSpritesheet("assets/walk/walk.png", *renderer,
                                      {getRect(0, 0, dudeWidth, dudeHeight),
                                       getRect(dudeWidth, 0, dudeWidth, dudeHeight),
                                       getRect(dudeWidth * 2, 0, dudeWidth, dudeHeight),
                                       getRect(dudeWidth * 3, 0, dudeWidth, dudeHeight)},
                                      true, &colorKey);
}

std::shared_ptr<Texture> Program::loadTexture(std::string const& name, bool flag,
                                              Color const* color)
{
    auto result = renderer->loadTexture(name, flag, color);
    return result;
}
