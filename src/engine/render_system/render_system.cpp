#include "render_system.h"

using namespace Mino;

std::shared_ptr<IRenderSystem> RenderSystem::create(Window& window)
{
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        std::cout << "RenderSystem could not be created! SDL Error:\n"
                  << SDL_GetError() << std::endl;
        throw std::runtime_error("RenderSystem could not be created!");
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return std::make_shared<RenderSystem>(renderer);
}

RenderSystem::RenderSystem(SDL_Renderer* renderer) : renderer(renderer) {}

RenderSystem::~RenderSystem() { SDL_DestroyRenderer(renderer); }

void RenderSystem::clear() { SDL_RenderClear(renderer); }

void RenderSystem::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    SDL_RenderCopy(renderer, texture.getRaw(), srcrect, dstrect);
}

void RenderSystem::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                          RotationData const& rotation)
{
    auto center = static_cast<SDL_Point>(rotation.center);
    SDL_RenderCopyEx(renderer, texture.getRaw(), srcrect, dstrect, rotation.angle, &center,
                     rotation.flip);
}

void RenderSystem::update()
{
    for (auto i = renderComponentRefs.begin(); i != renderComponentRefs.begin() + enabledRenderers;
         ++i)
    {
        (*i)->render();
    }
    SDL_RenderPresent(renderer);
}

std::shared_ptr<Texture> RenderSystem::loadTexture(std::string const& name, bool flag,
                                                   Color const* color)
{
    return Texture::loadTexture(name, *this, flag, *color);
}

void RenderSystem::setViewport(SDL_Rect* viewport) { SDL_RenderSetViewport(renderer, viewport); }

void IRenderSystem::removeRenderer(std::shared_ptr<RenderComponent> renderer)
{
    auto it = std::find(renderComponentRefs.begin(), renderComponentRefs.end(), renderer);
    if (it != renderComponentRefs.end())
    {
        renderComponentRefs.erase(it);
    }
}

void IRenderSystem::enableRenderer(std::shared_ptr<RenderComponent> renderer)
{
    /* TODO abstract this container */
    using std::iter_swap;

    auto first = renderComponentRefs.begin() + enabledRenderers;
    auto it = std::find(first, renderComponentRefs.end(), renderer);
    if (it != renderComponentRefs.end())
    {
        iter_swap(it, first);
        ++enabledRenderers;
    }
}

void IRenderSystem::disableRenderer(std::shared_ptr<RenderComponent> renderer)
{
    /* TODO abstract this */
    using std::iter_swap;

    auto last = renderComponentRefs.begin() + enabledRenderers;
    auto it = std::find(renderComponentRefs.begin(), last, renderer);
    if (it != last)
    {
        if (renderComponentRefs.size() > enabledRenderers)
            iter_swap(it, --last);
        else
            iter_swap(it, renderComponentRefs.rbegin());
        --enabledRenderers;
    }
}
