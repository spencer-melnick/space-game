#pragma once

#include "../math/vector2d.h"

namespace Game
{
    struct Texture
    {
        SDL_Texture* textureSdl;
        Vector2D dimensions;
    }
}
