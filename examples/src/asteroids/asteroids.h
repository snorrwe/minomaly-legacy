#pragma once
#include "asteroid_component.h"
#include "mino.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

const size_t SCREEN_WIDTH = 640;
const size_t SCREEN_HEIGHT = 480;

const size_t WORLD_WIDTH = 15000;

class Program : public Mino::Application
{

public:
    virtual void start();
};
