#include <iostream>

#include "system/window.h"
#include "math/vector2d.h"
#include "system/allocator.h"

void shutdown(Game::Window& window)
{
    window.destroy();
    SDL_Quit();
}

int main()
{
    std::cout << "Hello world!\n";

    Game::Allocator resourceBuffer;

    if (resourceBuffer.initialize(256) == false)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to allocate main resource buffer", nullptr);
        return 1;
    }

    Game::Window window;
    Game::Window::ErrorCode error;

    error = window.initialize({"Hello World", 640, 480, SDL_WINDOW_SHOWN});

    if (error != Game::Window::ErrorCode::NO_ERROR)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", Game::Window::getErrorString(error).c_str(), nullptr);
        shutdown(window);

        return 1;
    }

    auto one = resourceBuffer.allocate(Game::Vector2D({10.0f, 20.0f}));
    auto two = resourceBuffer.allocate(Game::Vector2D({9.0f, 19.0f}));

    std::cout << "Vector at " << one << " is " << one->x << ", " << one->y << "\n";
    std::cout << "Vector at " << two << " is " << two->x << ", " << two->y << "\n";

    SDL_Delay(3000);

    shutdown(window);

    return 0;
}

