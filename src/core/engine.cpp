#include <cassert>
#include <render/gl.h>
#include <core/window.h>
#include <ui/imgui.h>
#include <core/engine.h>

namespace Core {
    // create the Engine singleton
    Engine* Engine::create_instance() {
        assert(instance == nullptr);
        instance = new Engine;
        return instance;
    }

    // destroy the Engine singleton
    void Engine::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    // return the Engine singleton
    Engine* Engine::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    // initialize the Engine
    void Engine::init() {
        // create and initialize subsystems
        window = Core::Window::create_instance();
        window->init();
        imgui = UI::IMGUI::create_instance();
        imgui->init();

        // get our opengl function pointers
        Render::GL::get_function_pointers();
    }

    // clean up the Engine
    void Engine::cleanup() {
        // cleanup and destroy subsystems
        imgui->cleanup();
        UI::IMGUI::destroy_instance();
        window->cleanup();
        Core::Window::destroy_instance();
    }

    // run the Engine, starts the game loop
    void Engine::run() {
        isRunning = true;
        while(isRunning) {
            // process SDL events
            window->poll_events();

            // update viewport, clear buffers
            Render::GL::viewport_clear(window->get_window_width(), window->get_window_height());

            // tick subsystems
            imgui->tick();

            // swap buffers
            window->swap_buffers();
        }
    }

    // stop the Engine, breaks out of the game loop
    void Engine::stop() {
        isRunning = false;
    }
}