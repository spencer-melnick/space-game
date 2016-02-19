#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../math/vector2d.h"

namespace Game
{
    class Texture
    {
        public:
            SDL_Texture* textureSdl = nullptr;
            Vector2D dimensions = {0.0, 0.0};

            bool loadFromFile(const std::string& filename, const SDL_Renderer* renderer);
    };
}
