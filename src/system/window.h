#pragma once

#include <SDL2/SDL.h>

#include <string>

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
                VIDEO_INIT_FAILURE,
                WINDOW_CREATION_FAILURE
            };

            static std::string getErrorString(const ErrorCode error);
            ErrorCode initialize(const Config config);
            void destroy();

        private:
            SDL_Window* _windowSdl = nullptr;
            SDL_Surface* _surfaceSdl = nullptr;
    };

}
