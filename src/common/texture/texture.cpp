#include "texture.h"

std::shared_ptr<Texture> Texture::loadTexture(std::string const& name, IRenderer& renderer,
                                              bool flag, Color const* color)
{
    auto media = IMG_Load(name.c_str());
    if (media == nullptr)
    {
        std::cout << "Unable to load image [" << name << "] SDL Error: \n"
                  << IMG_GetError() << std::endl;
        throw std::runtime_error("Unable to load image");
    }
    if (flag)
    {
        uint32_t key = color ? SDL_MapRGB(media->format, color->red, color->green, color->blue)
                             : SDL_MapRGB(media->format, 0, 0, 0);
        SDL_SetColorKey(media, SDL_TRUE, key);
    }
    auto texture = SDL_CreateTextureFromSurface(renderer.getRaw(), media);
    if (texture == nullptr)
    {
        std::cout << "Unable to optimize image! SDL Error: \n" << SDL_GetError() << std::endl;
        throw std::runtime_error("Unable to optimize image");
    }
    auto result = std::make_shared<Texture>(std::make_shared<ManagedTexture>(texture), media->w,
                                            media->h, renderer);
    SDL_FreeSurface(media);
    return result;
}

Texture::TSpriteSheet Texture::loadSpritesheet(std::string const& name, IRenderer& renderer,
                                               std::vector<SDL_Rect> const& rects, bool flag,
                                               Color const* colorKey)
{
    auto texture = loadTexture(name, renderer, flag, colorKey);
    auto result = TSpriteSheet();
    for (auto i = rects.begin(); i != rects.end(); ++i)
    {
        result.push_back(std::make_shared<Texture>(*texture));
        result.back()->setSrcrect(std::make_shared<SDL_Rect>(*i));
    }
    return result;
}

Texture::Texture(std::shared_ptr<ManagedTexture> texture, int width, int height,
                 IRenderer& renderer)
    : texture(texture), width(width), height(height), renderer(renderer)
{
}

void Texture::render(Vector2 const& pos) const
{
    render(pos, [&](Texture const& t, SDL_Rect* srcrect, SDL_Rect* dstrect) {
        renderer.render(t, srcrect, dstrect);
    });
}

void Texture::render(Vector2 const& pos, ICamera& camera) const
{
    render(pos, [&](Texture const& t, SDL_Rect* srcrect, SDL_Rect* dstrect) {
        camera.render(t, srcrect, dstrect);
    });
}

void Texture::render(Vector2 const& pos,
                     std::function<void(Texture const&, SDL_Rect*, SDL_Rect*)> renderfn) const
{
    SDL_Rect renderQuad = {pos.x, pos.y, width, height};
    renderfn(*this, srcrect.get(), &renderQuad);
}

void Texture::render(Vector2 const& pos, RotationData const& rotation) const
{
    render(pos, [&](Texture const& t, SDL_Rect* srcrect, SDL_Rect* dstrect) {
        renderer.render(t, srcrect, dstrect, rotation);
    });
}

void Texture::render(Vector2 const& pos, ICamera& camera, RotationData const& rotation) const
{
    render(pos, [&](Texture const& t, SDL_Rect* srcrect, SDL_Rect* dstrect) {
        camera.render(t, srcrect, dstrect, rotation);
    });
}

void Texture::color(Color const& color)
{
    SDL_SetTextureColorMod(*texture, color.red, color.green, color.blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) { SDL_SetTextureBlendMode(*texture, blending); }
void Texture::setAlpha(uint8_t alpha) { SDL_SetTextureAlphaMod(*texture, alpha); }

void Texture::setSrcrect(std::shared_ptr<SDL_Rect> rect)
{
    srcrect = rect;
    width = rect->w;
    height = rect->h;
}
