#pragma once
#include "component.h"
#include "render_system.h"
#include <memory>

namespace Mino
{

class IRenderSystem;

class RendererComponent : public Component
{
public:
    template <typename TRender> static std::shared_ptr<TRender> create(GameObject* gameObject);

    virtual ~RendererComponent() {}

    virtual void render() = 0;

    virtual void enable();
    virtual void disable();

protected:
    std::weak_ptr<IRenderSystem> renderSystem;
};

template <typename TRender>
std::shared_ptr<TRender> RendererComponent::create(GameObject* gameObject)
{
    static_assert(std::is_convertible<TRender*, RendererComponent*>::value);

    auto renderSystem = gameObject->getScene()->getEngineCore()->getRenderer();
    auto result = renderSystem->createRenderer<TRender>();
    result->gameObject = gameObject;
    result->self = std::weak_ptr<Component>(result);
    result->renderSystem = renderSystem;
    result->start();
    return result;
}

} // namespace Mino
