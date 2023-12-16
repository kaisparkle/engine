#include <cassert>
#include <chrono>
#include <random>
#include <optick.h>
#include <render/gl.h>
#include <core/window.h>
#include <ui/editor.h>
#include <tools/modelimport.h>
#include <components/mesh.h>
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
        editor = UI::Editor::create_instance();
        editor->init();
        renderer = Render::RendererGL::create_instance();
        renderer->init();
        entityManager = Entity::EntityManager::create_instance();
        entityManager->init();
        playerManager = Player::PlayerManager::create_instance();
        playerManager->init();
        deltatime = Core::DeltaTime::create_instance();
        deltatime->init();

        Tools::import_model("../assets/SciFiHelmet.gltf");
        Tools::import_model("../assets/sponza-gltf-pbr/sponza.glb");
    }

    // clean up the Engine
    void Engine::cleanup() {
        // cleanup and destroy subsystems
        editor->cleanup();
        UI::Editor::destroy_instance();
        window->cleanup();
        Core::Window::destroy_instance();
        renderer->cleanup();
        Render::RendererGL::destroy_instance();
        entityManager->cleanup();
        Entity::EntityManager::destroy_instance();
        playerManager->cleanup();
        Player::PlayerManager::destroy_instance();
        deltatime->cleanup();
        Core::DeltaTime::destroy_instance();
    }

    // run the Engine, starts the game loop
    void Engine::run() {
        isRunning = true;
        while(isRunning) {
            OPTICK_FRAME("MainThread");
            OPTICK_EVENT();
            deltatime->start_frame();

            // process SDL events
            window->poll_events();

            // update viewport
            renderer->resize_viewport(window->get_window_width(), window->get_window_height());

            // tick subsystems
            renderer->tick();
            editor->tick();

            // swap buffers
            window->swap_buffers();

            deltatime->end_frame();
        }
    }

    // stop the Engine, breaks out of the game loop
    void Engine::stop() {
        isRunning = false;
    }
}