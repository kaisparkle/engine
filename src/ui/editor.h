#pragma once

#include <SDL.h>

namespace UI {
    class Editor {
    public:
        // create the Editor singleton
        static Editor* create_instance();
        // destroy the Editor singleton
        static void destroy_instance();
        // return the Editor singleton
        static Editor* get_instance();

        // initialize the Editor
        void init();
        // clean up the Editor
        void cleanup();

        // process SDL2 events
        void process_event(SDL_Event* event);
        // draw the next frame
        void tick();
    private:
        inline static Editor* instance;

        // frametime plot
        void frametime_plot();
        // scene pane
        void scene_pane();
    };
}