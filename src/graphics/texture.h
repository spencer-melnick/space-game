#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../math/vector2d.h"
#include "../system/resource.h"

namespace Game
{
    class Texture
    {
        public:
            enum class ErrorCode
            {
                NO_ERROR,
                INVALID_RENDERER,
                LOAD_FAILURE,
                SURFACE_TO_TEXTURE_FAILURE
            };

            SDL_Texture* textureSdl = nullptr;
            Vector2D<int> dimensions = {0, 0};

            ErrorCode loadFromFile(const std::string& filename, SDL_Renderer* renderer);

            static ResourceType resourceType;

            static void deallocate(void* data, size_t elements);
    };
}
