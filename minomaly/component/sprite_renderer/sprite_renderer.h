#pragma once
#include "application.h"
#include "component.h"
#include "fixed_matrix.h"
#include "renderer.h"
#include "texture.h"
#include <iostream>
#include <memory>

namespace Mino
{

class SpriteRendererComponent : public RenderComponent
{
public:
    virtual void update();
    virtual void render(Transform::TransformRef const& camera);

    void clearTexture();
    void setTexture(Texture* t);
    Texture* getTexture() { return texture; }

protected:
    int height = 0;
    int width = 0;
    Texture* texture = nullptr;
};

template <>
std::unique_ptr<SpriteRendererComponent>
Component::create<SpriteRendererComponent>(GameObject& gameObject);

} // namespace Mino
