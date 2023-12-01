#pragma once

#include <core/window.h>
#include <ui/imgui.h>

namespace Core {
    class Engine {
    public:
        static Engine* create_instance();
        static void destroy_instance();
        static Engine* get_instance();

        void init();
        void cleanup();

        void run();
        void stop();
    private:
        inline static Engine* instance;
        bool isRunning = false;

        // singletons
        Core::Window* window;
        UI::IMGUI* imgui;
    };
}