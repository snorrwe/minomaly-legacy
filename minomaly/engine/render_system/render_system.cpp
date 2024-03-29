#include "render_system.h"

using namespace Mino;

std::unique_ptr<IRenderSystem> RenderSystem::create(WindowSystem& window)
{
    auto renderer
        = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
    {
        std::cout << "RenderSystem could not be created! SDL Error:\n"
                  << SDL_GetError() << std::endl;
        throw std::runtime_error("RenderSystem could not be created!");
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    auto result = std::make_unique<RenderSystem>(renderer);
    auto& mainCamera = result->getMainCamera();
    auto win = window.size();
    mainCamera->setViewport(0, 0, win.x(), win.y());
    return std::move(result);
}

RenderSystem::RenderSystem(SDL_Renderer* renderer)
    : renderer(renderer)
{
    mainCamera = addCamera();
}

RenderSystem::~RenderSystem() { SDL_DestroyRenderer(renderer); }

void RenderSystem::clear() { SDL_RenderClear(renderer); }

void RenderSystem::render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    SDL_RenderCopy(renderer, texture.getRaw(), srcrect, dstrect);
}

void RenderSystem::render(Texture const& texture,
                          SDL_Rect* srcrect,
                          SDL_Rect* dstrect,
                          RotationData const& rotation)
{
    SDL_Point center{rotation.center.x(), rotation.center.y()};
    SDL_RenderCopyEx(renderer,
                     texture.getRaw(),
                     srcrect,
                     dstrect,
                     rotation.angle * 57.2958,
                     &center,
                     rotation.flip);
}

void RenderSystem::update()
{
    clear();
    cameras.foreachActive([&](auto& camera) {
        setViewport(&camera.getViewport());
        auto last = renderComponentRefs.begin() + enabledRenderers;
        for (auto i = renderComponentRefs.begin(); i != last; ++i)
        {
            auto& renderComponent = *i;
            renderComponent->render(camera.getTransform());
        }
    });
    SDL_RenderPresent(renderer);
}

std::shared_ptr<Texture>
RenderSystem::loadTexture(std::string const& name, bool flag, Color const* color)
{
    return Texture::loadTexture(name, *this, flag, *color);
}

void RenderSystem::setViewport(SDL_Rect* viewport) { SDL_RenderSetViewport(renderer, viewport); }

void IRenderSystem::removeRenderer(RenderComponent* renderer)
{
    auto it = std::find(renderComponentRefs.begin(), renderComponentRefs.end(), renderer);
    if (it != renderComponentRefs.end())
    {
        renderComponentRefs.erase(it);
    }
}

void IRenderSystem::enableRenderer(RenderComponent* renderer)
{
    using std::iter_swap;

    auto first = renderComponentRefs.begin() + enabledRenderers;
    auto it = std::find(first, renderComponentRefs.end(), renderer);
    if (it != renderComponentRefs.end())
    {
        iter_swap(it, first);
        ++enabledRenderers;
    }
}

void IRenderSystem::disableRenderer(RenderComponent* renderer)
{
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

typename RenderSystem::CameraReferences::Reference RenderSystem::addCamera()
{
    return cameras.enable();
}
