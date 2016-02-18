#include <iostream>

#include "system/window.h"
#include "math/vector2d.h"
#include "system/resource.h"

void shutdown(Game::Window& window)
{
    window.destroy();
    SDL_Quit();
}

int main()
{
    std::cout << "Hello world!\n";

    Game::ResourceManager resourceBuffer;

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

    {
        std::cout << resourceBuffer.getNumberResources() << " resources headers stored\n";

        auto one = resourceBuffer.allocateResource("Vector1", Game::ResourceType::DATA, Game::Vector2D({10.0f, 20.0f}));
        std::cout << resourceBuffer.getNumberResources() << " resources headers stored\n";
        resourceBuffer.allocateResource("Vector2", Game::ResourceType::DATA, Game::Vector2D({9.0f, 19.0f}));
        std::cout << resourceBuffer.getNumberResources() << " resources headers stored\n";

        auto rawOne = static_cast<Game::Vector2D*>(one.getData());

        std::cout << "Vectors are now " << ((one.isValid()) ? "valid" : "invalid") << "\n";
        std::cout << "Vector at " << rawOne << " is " << rawOne->x << ", " << rawOne->y << "\n";

        resourceBuffer.reset();

        std::cout << "Vectors are now " << ((one.isValid()) ? "valid" : "invalid") << "\n";

        std::cout << resourceBuffer.getNumberResources() << " resources headers stored\n";
    }

    std::cout << resourceBuffer.getNumberResources() << " resources headers stored\n";

    SDL_Delay(3000);

    shutdown(window);

    return 0;
}

