#include <iostream>

#include "system/window.h"
#include "math/vector2d.h"
#include "system/resource.h"
#include "graphics/texture.h"

using namespace Game;

void shutdown(Window& window)
{
    window.destroy();
    SDL_Quit();
}

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
        shutdown(window);

        return 1;
    }

    auto textureHandle1 = resourceBuffer.allocateResource<Texture>("Texture1", ResourceType::TEXTURE);
    auto textureHandle2 = resourceBuffer.allocateResource<Texture>("Texture2", ResourceType::TEXTURE);

    SDL_Delay(3000);

    shutdown(window);

    return 0;
}

