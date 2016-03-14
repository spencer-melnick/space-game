#include <iostream>
#include <type_traits>

#include "system/window.h"
#include "math/vector2d.h"
#include "system/resource.h"
#include "system/pool.h"
#include "graphics/texture.h"

using namespace Game;

int main()
{
    std::cout << "Hello world!\n";

    ResourceBuffer resourceBuffer;

    if (resourceBuffer.initialize(1 * MiB, 20) == false)
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


    auto poolData = new ObjectPool<int>::Object[10];
    ObjectPool<int> pool(poolData, 10);

    for (int i = 0; i < 10; i ++)
    {
        auto a = pool.createObject();
        if (a.isValid())
            *(a.getData()) = i;
    }

    auto i = pool.getUsedObjects();
    auto j = i.getNextObject().getNextObject().getNextObject();

    pool.freeObject(j);

    i = pool.getUsedObjects();
    while (i.isValid())
    {
        std::cout << *(i.getData()) << " ";
        i = i.getNextObject();
    }


    SDL_Rect position({0, 0, texture1->dimensions.x, texture1->dimensions.y});

    SDL_RenderClear(window.getSdlRenderer());

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

