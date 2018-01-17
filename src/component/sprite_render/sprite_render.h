#pragma once
#include "component.h"
#include "renderer.h"
#include "scene.h"
#include "texture.h"
#include <iostream>
#include <memory>

namespace Mino
{

class SpriteRenderComponent : public RenderComponent
{
public:
    virtual void start();
    virtual void update();
    virtual void render(Transform::TransformRef camera = nullptr);

    void clearTexture();
    void setTexture(std::shared_ptr<Texture> t);
    std::shared_ptr<Texture> getTexture() { return texture; }

protected:
    Transform::TransformRef transform = nullptr;
    std::shared_ptr<Texture> texture = nullptr;
};

template <>
std::shared_ptr<SpriteRenderComponent>
Component::create<SpriteRenderComponent>(GameObject* gameObject);

} // namespace Mino
