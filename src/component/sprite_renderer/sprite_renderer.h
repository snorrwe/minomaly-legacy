#pragma once
#include "application.h"
#include "component.h"
#include "renderer.h"
#include "texture.h"
#include <iostream>
#include <memory>

namespace Mino
{

class SpriteRendererComponent : public RenderComponent
{
public:
    virtual void start();
    virtual void update();
    virtual void render(Transform::TransformRef camera = nullptr);

    void clearTexture();
    void setTexture(Texture* t);
    Texture* getTexture() { return texture; }

protected:
    int height = 0;
    Transform::TransformRef transform = nullptr;
    Texture* texture = nullptr;
};

template <>
std::unique_ptr<SpriteRendererComponent>
Component::create<SpriteRendererComponent>(GameObject* gameObject);

} // namespace Mino
