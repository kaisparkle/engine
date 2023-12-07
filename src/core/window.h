#pragma once

#include <SDL.h>

namespace Core {
    class Window {
    public:
        static Window* create_instance();
        static void destroy_instance();
        static Window* get_instance();

        void init();
        void cleanup();

        SDL_Window* get_handle();
        SDL_GLContext get_glcontext();

        int get_window_width();
        int get_window_height();

        void poll_events();

        void swap_buffers();
    private:
        inline static Window* instance;
        SDL_Window* handle = nullptr;
        SDL_GLContext glContext = nullptr;

        // TODO: cvars?
        int window_width = 1600;
        int window_height = 900;
    };
}