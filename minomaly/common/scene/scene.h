#pragma once
#include "json.h"
#include <string>
#include <tuple>
#include <vector>

namespace Mino
{
struct Scene
{
    std::vector<std::string> audioFiles = {};
    std::vector<std::string> spriteSheetFiles = {};
    std::vector<std::string> animationFiles = {};

    constexpr static auto jsonProperties()
    {
        return std::make_tuple(Json::property(&Scene::audioFiles, "audioFiles"),
                               Json::property(&Scene::spriteSheetFiles, "spriteSheetFiles"),
                               Json::property(&Scene::animationFiles, "animationFiles"));
    }
};
} // namespace Mino

