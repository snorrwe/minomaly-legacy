#pragma once
#include "SDL.h"
#include "observer.h"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

class Input
{
public:
    static std::shared_ptr<Input> create();

    Input() = default;
    Input(Input const&) = delete;
    Input(Input&&) = delete;

    Input& operator=(Input const&) = delete;
    Input& operator=(Input&&) = delete;

    std::vector<SDL_Event>& getEvents() { return events; }

    void update();
    bool isDown(SDL_Keycode const&);
    bool isUp(SDL_Keycode const&);

    ISubscription onQuit(std::function<void(uint8_t)> cb);
    ISubscription onKeyDown(std::function<void(SDL_Event)>);
    ISubscription onKeyUp(std::function<void(SDL_Event)>);

private:
    Subject<uint8_t> onQuitSubject;
    Subject<SDL_Event> onKeyDownSubject;
    Subject<SDL_Event> onKeyUpSubject;
    std::vector<SDL_Event> events;
    std::map<size_t, bool> keysDown;
};
