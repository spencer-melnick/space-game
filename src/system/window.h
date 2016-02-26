#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#define IMAGE_FLAGS (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)

namespace Game
{

    class Window
    {
        public:
            struct Config
            {
                std::string title;
                int width;
                int height;
                Uint32 flags;
            };

            enum class ErrorCode
            {
                NO_ERROR,
                SDL_INIT_FAILURE,
                WINDOW_CREATION_FAILURE,
                RENDERER_CREATION_FAILURE,
                IMAGE_INIT_FAILURE
            };

            static std::string getErrorString(const ErrorCode error);
            ErrorCode initialize(const Config config);
            SDL_Renderer* getSdlRenderer() const;
            void destroy();

        private:
            SDL_Window* _windowSdl = nullptr;
            SDL_Surface* _surfaceSdl = nullptr;
            SDL_Renderer* _rendererSdl = nullptr;
    };

}
