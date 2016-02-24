#include "window.h"

using namespace Game;

std::string Window::getErrorString(const ErrorCode error)
{
    switch(error)
    {
        case ErrorCode::NO_ERROR:
            return std::string("No error occurred");
        case ErrorCode::VIDEO_INIT_FAILURE:
            return std::string("SDL failed to initialize the video subsystem: ") + SDL_GetError();
        case ErrorCode::WINDOW_CREATION_FAILURE:
            return std::string("Window creation failed: ") + SDL_GetError();
        case ErrorCode::RENDERER_CREATION_FAILURE:
            return std::string("SDL Renderer creation failed: ") + SDL_GetError();
        default:
            return std::string("SDL error: ") + SDL_GetError();
    }
}

Window::ErrorCode Window::initialize(const Config config)
{
    if (SDL_VideoInit(nullptr) != 0)
        return ErrorCode::VIDEO_INIT_FAILURE;

    _windowSdl = SDL_CreateWindow(config.title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  config.width, config.height, config.flags);

    if (_windowSdl == nullptr)
        return ErrorCode::WINDOW_CREATION_FAILURE;

    _surfaceSdl = SDL_GetWindowSurface(_windowSdl);
    _rendererSdl = SDL_CreateRenderer(_windowSdl, -1, RENDERER_FLAGS);

    if (_rendererSdl == nullptr)
    {
        destroy();
        return ErrorCode::RENDERER_CREATION_FAILURE;
    }

    if (IMG_Init(IMAGE_FLAGS) != IMAGE_FLAGS)
    {
        //error
    }

    return ErrorCode::NO_ERROR;
}

const SDL_Renderer* Window::getSdlRenderer() const
{
    return _rendererSdl;
}

void Window::destroy()
{
    if (_rendererSdl != nullptr)
    {
        SDL_DestroyRenderer(_rendererSdl);
        _rendererSdl = nullptr;
    }

    if (_windowSdl != nullptr)
    {
        SDL_DestroyWindow(_windowSdl);
        _windowSdl = nullptr;
    }
}
