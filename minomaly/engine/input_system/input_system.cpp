#include "input_system.h"

using namespace Mino;

std::unique_ptr<Input> Input::create() { return std::make_unique<Input>(); }

void Input::update()
{
    events.clear();
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        events.push_back(e);

        switch (e.type)
        {
        case SDL_QUIT:
            onQuitSubject.next(0);
            break;
        case SDL_KEYDOWN:
            keysDown[static_cast<size_t>(e.key.keysym.sym)] = true;
            onKeyDownSubject.next(e);
            break;
        case SDL_KEYUP:
            keysDown[static_cast<size_t>(e.key.keysym.sym)] = false;
            onKeyUpSubject.next(e);
            break;
        case SDL_MOUSEMOTION:
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            currentMousePosition = Vector2<int>(x, y);
            break;
        }
        }
    }
}

bool Input::isDown(SDL_Keycode const& code) { return keysDown[static_cast<size_t>(code)]; }

bool Input::isUp(SDL_Keycode const& code) { return !keysDown[static_cast<size_t>(code)]; }

ISubscription Input::onQuit(std::function<void(uint8_t)> cb) { return onQuitSubject.subscribe(cb); }

ISubscription Input::onKeyDown(std::function<void(SDL_Event)> cb)
{
    return onKeyDownSubject.subscribe(cb);
}

ISubscription Input::onKeyUp(std::function<void(SDL_Event)> cb)
{
    return onKeyUpSubject.subscribe(cb);
}

Vector2<int> Input::mouseScreenPosition() { return currentMousePosition; }
