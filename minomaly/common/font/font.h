#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "asset.h"
#include <iostream>
#include <memory>
#include <string>

namespace Mino
{
class Font : public Asset
{
public:
    static std::shared_ptr<Font> load(std::string const& path, int pts);

    Font(TTF_Font* font) : font(font) {}
    ~Font() { TTF_CloseFont(font); }

    operator TTF_Font*() const { return font; }

private:
    TTF_Font* font;
};
} // namespace Mino
