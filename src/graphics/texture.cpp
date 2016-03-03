#include "texture.h"

using namespace Game;

ResourceType Texture::resourceType = ResourceType::TEXTURE;

Texture::ErrorCode Texture::loadFromFile(const std::string& filename, SDL_Renderer* renderer)
{
    if (renderer == nullptr)
        return ErrorCode::INVALID_RENDERER;

    SDL_Surface* surface = IMG_Load(filename.c_str());

    if (surface == nullptr)
        return ErrorCode::LOAD_FAILURE;

    textureSdl = SDL_CreateTextureFromSurface(renderer, surface);

    if (textureSdl == nullptr)
        return ErrorCode::SURFACE_TO_TEXTURE_FAILURE;


    int w, h;
    SDL_QueryTexture(textureSdl, nullptr, nullptr, &w, &h);

    dimensions.x = static_cast<float>(w);
    dimensions.y = static_cast<float>(h);

    return ErrorCode::NO_ERROR;
}

void Texture::deallocate(void* data, size_t elements)
{
    Texture* texture = static_cast<Texture*>(data);
    Texture* temp;

    for (unsigned int i = 0; i < elements; i ++)
    {
        temp = texture + i;

        if (temp->textureSdl != nullptr)
            SDL_DestroyTexture(temp->textureSdl);
    }
}
