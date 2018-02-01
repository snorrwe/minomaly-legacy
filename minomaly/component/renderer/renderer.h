#pragma once
#include "component.h"
#include "game_object.h"
#include "render_system.h"
#include "transform.h"
#include <memory>

namespace Mino
{

class IRenderSystem;
class Component;
class GameObject;

class RenderComponent : public Component
{
public:
    template <typename TRender> static std::unique_ptr<TRender> create(GameObject& gameObject);

    virtual ~RenderComponent();

    virtual void render(Transform::TransformRef const& camera) = 0;

    virtual void enable();
    virtual void disable();

protected:
    IRenderSystem* renderSystem;
};

template <typename TRender> std::unique_ptr<TRender> RenderComponent::create(GameObject& gameObject)
{
    static_assert(std::is_convertible<TRender*, RenderComponent*>::value);

    auto renderSystem = gameObject.getApplication()->getEngineCore()->getRenderer();
    auto result = renderSystem->createRenderer<TRender>();
    result->gameObject = &gameObject;
    result->setTransform(gameObject.getTransform());
    result->renderSystem = renderSystem;
    result->start();
    return std::move(result);
}

} // namespace Mino
