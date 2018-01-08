#pragma once
#include "SDL.h"
#include "color.h"
#include "core.h"
#include "renderdata.h"
#include "renderer.h"
#include "texture.h"
#include "window.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

namespace Mino
{

class Core;
class Texture;
class RenderComponent;
class Window;

class IRenderSystem
{
public:
    friend class Core;

    using RenderComponentReferences = std::vector<std::shared_ptr<RenderComponent>>;
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    virtual ~IRenderSystem() {}

    virtual void render(Texture const& texture, SDL_Rect* srcrect = nullptr,
                        SDL_Rect* dstrect = nullptr) = 0;
    virtual void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                        RotationData const& rotation) = 0;
    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const* color = nullptr) = 0;
    virtual void setViewport(SDL_Rect* viewport) = 0;
    virtual void clear() = 0;
    virtual void update() = 0;

    virtual SDL_Renderer* getRaw() = 0;

    template <typename TRenderer> std::shared_ptr<TRenderer> createRenderer();
    void removeRenderer(std::shared_ptr<RenderComponent>);
    void enableRenderer(std::shared_ptr<RenderComponent>);
    void disableRenderer(std::shared_ptr<RenderComponent>);

protected:
    RenderComponentReferences renderComponentRefs;
    size_t enabledRenderers = 0;
};

class RenderSystem : public IRenderSystem
{
public:
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    static std::shared_ptr<IRenderSystem> create(Window& window);

    RenderSystem(SDL_Renderer* renderer);
    RenderSystem(RenderSystem const&) = default;
    RenderSystem(RenderSystem&&) = default;
    virtual ~RenderSystem();

    RenderSystem& operator=(RenderSystem const&) = default;
    RenderSystem& operator=(RenderSystem&&) = default;

    virtual void render(Texture const& texture, SDL_Rect* srcrect = nullptr,
                        SDL_Rect* dstrect = nullptr);
    virtual void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                        RotationData const& rotation);
    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool flag = false,
                                                 Color const* color = nullptr);
    virtual void setViewport(SDL_Rect* viewport);
    virtual void clear();
    virtual void update();

    virtual SDL_Renderer* getRaw() { return renderer; }

private:
    SDL_Renderer* renderer;
};

template <typename TRenderer> std::shared_ptr<TRenderer> IRenderSystem::createRenderer()
{
    static_assert(std::is_convertible<TRenderer*, RenderComponent*>::value);
    auto result = std::make_shared<TRenderer>();
    renderComponentRefs.insert(renderComponentRefs.begin() + enabledRenderers, result);
    ++enabledRenderers;
    return result;
}

} // namespace Mino
