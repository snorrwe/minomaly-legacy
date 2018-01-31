#pragma once
#include "SDL.h"
#include "camera.h"
#include "color.h"
#include "core.h"
#include "iterable_pool.h"
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

class EngineCore;
class Texture;
class RenderComponent;
class WindowSystem;

class IRenderSystem
{
public:
    //friend class EngineCore;

    using RenderComponentReferences = std::vector<RenderComponent*>;
    using CameraReferences = Camera::CameraReferences;
    using CameraReference = Camera::CameraReference;
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    virtual ~IRenderSystem() {}

    virtual void render(Texture const& texture, SDL_Rect* srcrect = nullptr,
                        SDL_Rect* dstrect = nullptr) = 0;
    virtual void render(Texture const& texture, SDL_Rect* srcrect, SDL_Rect* dstrect,
                        RotationData const& rotation) = 0;
    virtual std::shared_ptr<Texture> loadTexture(std::string const& name, bool enableColorKeying = false,
                                                 Color const* color = nullptr) = 0;
    virtual void setViewport(SDL_Rect* viewport) = 0;
    virtual void update() = 0;
    virtual CameraReference addCamera() = 0;
    virtual CameraReference& getMainCamera() = 0;
    virtual CameraReference const& getMainCamera() const = 0;

    virtual SDL_Renderer* getRaw() = 0;

    template <typename TRenderer> std::unique_ptr<TRenderer> createRenderer();
    void removeRenderer(RenderComponent*);
    void enableRenderer(RenderComponent*);
    void disableRenderer(RenderComponent*);

protected:
    RenderComponentReferences renderComponentRefs;
    size_t enabledRenderers = 0;
};

class RenderSystem : public IRenderSystem
{
public:
    using RotationData = RenderData::RotationData;
    using Vector2 = RenderData::Vector2;

    static std::shared_ptr<IRenderSystem> create(WindowSystem& window);

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
    virtual void update();

    virtual CameraReference addCamera();
    virtual CameraReference& getMainCamera() { return mainCamera; }
    virtual CameraReference const& getMainCamera() const { return mainCamera; }

    virtual SDL_Renderer* getRaw() { return renderer; }

protected:
    CameraReferences cameras;
    CameraReference mainCamera;

private:
    void clear();

    SDL_Renderer* renderer;
};

template <typename TRenderer> std::unique_ptr<TRenderer> IRenderSystem::createRenderer()
{
    static_assert(std::is_convertible<TRenderer*, RenderComponent*>::value);
    auto result = std::make_unique<TRenderer>();
    renderComponentRefs.insert(renderComponentRefs.begin() + enabledRenderers, result.get());
    ++enabledRenderers;
    return std::move(result);
}

} // namespace Mino
