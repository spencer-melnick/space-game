#include <iostream>
#include <type_traits>

#include "system/window.h"
#include "math/vector2d.h"
#include "system/resource.h"
#include "graphics/texture.h"

using namespace Game;

int main()
{
    std::cout << "Hello world!\n";

    ResourceBuffer resourceBuffer;

    if (resourceBuffer.initialize(1 * KiB) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to allocate main resource buffer", nullptr);
        return 1;
    }

    Window window;
    Window::ErrorCode error;

    error = window.initialize({"Hello World", 640, 480, SDL_WINDOW_SHOWN});

    if (error != Window::ErrorCode::NO_ERROR)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", Window::getErrorString(error).c_str(), nullptr);

        return 1;
    }

    auto textureHandle1 = resourceBuffer.allocateResource<Texture>("Texture1");

    auto texture1 = textureHandle1.getData<Texture>();
    Texture::ErrorCode loadError = texture1->loadFromFile("./rc/texture.png", window.getSdlRenderer());

    SDL_Rect position({0, 0, texture1->dimensions.x, texture1->dimensions.y});

    if (loadError != Texture::ErrorCode::NO_ERROR)
        std::cout << "\"./rc/texture.png\" failed to load\n";
    else
        SDL_RenderCopy(window.getSdlRenderer(), texture1->textureSdl, nullptr, &position);

    SDL_RenderPresent(window.getSdlRenderer());

    SDL_Delay(3000);

    resourceBuffer.destroy();
    window.destroy();

    return 0;
}

