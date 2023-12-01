#pragma once

#include <SDL.h>

namespace UI {
    class IMGUI {
    public:
        static IMGUI* create_instance();
        static void destroy_instance();
        static IMGUI* get_instance();

        void init();
        void cleanup();

        void process_event(SDL_Event* event);
        void tick();
    private:
        inline static IMGUI* instance;
    };
}