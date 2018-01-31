#pragma once

namespace Mino::Private
{

struct ManagedTexture
{
    ManagedTexture(SDL_Texture* texture) : texture(texture) {}
    ManagedTexture(ManagedTexture const&) = delete;
    ManagedTexture(ManagedTexture&&) = delete;
    ~ManagedTexture() { SDL_DestroyTexture(texture); }

    ManagedTexture& operator=(ManagedTexture const&) = delete;
    ManagedTexture& operator=(ManagedTexture&&) = delete;

    operator SDL_Texture*() { return texture; }

    SDL_Texture* texture;
};

} // namespace Mino::Private
