#pragma once
#include "component.h"
#include "renderer.h"
#include "scene.h"
#include "texture.h"
#include <memory>

namespace Mino
{

class SpriteRenderComponent : public RenderComponent
{
public:
    virtual void start();
    virtual void render();

    void clearTexture();
    void setTexture(std::shared_ptr<Texture> t);
    std::shared_ptr<Texture> getTexture() { return texture; }

protected:
    Transform* transform;
    std::shared_ptr<Texture> texture = nullptr;
};

} // namespace Mino
