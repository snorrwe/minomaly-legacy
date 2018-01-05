#include "renderer.h"

using namespace Mino;

std::shared_ptr<IRenderSystem> Renderer::create(SDL_Window* window)
{
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "Renderer could not be created! SDL Error:\n" << SDL_GetError() << std::endl;
        throw std::runtime_error("Renderer could not be created!");
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return std::make_shared<Renderer>(renderer);
}

Renderer::Renderer(SDL_Renderer* renderer) : renderer(renderer) {}

Renderer::~Renderer() { SDL_DestroyRenderer(renderer); }

void Renderer::clear() { SDL_RenderClear(renderer); }

void Renderer::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    SDL_RenderCopy(renderer, texture.getRaw(), srcrect, dstrect);
}

void Renderer::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                      RotationData const& rotation)
{
    auto center = static_cast<SDL_Point>(rotation.center);
    SDL_RenderCopyEx(renderer, texture.getRaw(), srcrect, dstrect, rotation.angle, &center,
                     rotation.flip);
}

void Renderer::update() { SDL_RenderPresent(renderer); }

std::shared_ptr<Texture> Renderer::loadTexture(std::string const& name, bool flag,
                                               Color const* color)
{
    return Texture::loadTexture(name, *this, flag, *color);
}

void Renderer::setViewport(SDL_Rect* viewport) { SDL_RenderSetViewport(renderer, viewport); }
