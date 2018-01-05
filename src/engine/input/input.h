#pragma once
#include "SDL.h"
#include "observer.h"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

class IInputSystem
{
public:
    virtual ~IInputSystem() {}

    virtual void update() = 0;
    virtual bool isDown(SDL_Keycode const&) = 0;
    virtual bool isUp(SDL_Keycode const&) = 0;
    virtual ISubscription onQuit(std::function<void(uint8_t)> cb) = 0;
    virtual ISubscription onKeyDown(std::function<void(SDL_Event)>) = 0;
    virtual ISubscription onKeyUp(std::function<void(SDL_Event)>) = 0;
};

class Input : public IInputSystem
{
public:
    static std::shared_ptr<Input> create();

    Input() = default;
    Input(Input const&) = delete;
    Input(Input&&) = delete;
    virtual ~Input() {}

    Input& operator=(Input const&) = delete;
    Input& operator=(Input&&) = delete;

    std::vector<SDL_Event>& getEvents() { return events; }

    virtual void update();
    virtual bool isDown(SDL_Keycode const&);
    virtual bool isUp(SDL_Keycode const&);

    virtual ISubscription onQuit(std::function<void(uint8_t)> cb);
    virtual ISubscription onKeyDown(std::function<void(SDL_Event)>);
    virtual ISubscription onKeyUp(std::function<void(SDL_Event)>);

private:
    Subject<uint8_t> onQuitSubject;
    Subject<SDL_Event> onKeyDownSubject;
    Subject<SDL_Event> onKeyUpSubject;
    std::vector<SDL_Event> events;
    std::map<size_t, bool> keysDown;
};
