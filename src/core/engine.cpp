#include <cassert>
#include <chrono>
#include <random>
#include <optick.h>
#include <render/gl/renderer.h>
#include <core/window.h>
#include <ui/editor.h>
#include <tools/modelimport.h>
#include <component/rigidbody.h>
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
        renderer = Render::RendererGL::create_instance();
        renderer->init();
        renderer->resize_viewport(window->get_window_width(), window->get_window_height());
        editor = UI::Editor::create_instance();
        editor->init(renderer->get_framebuffer());
        entityManager = Entity::EntityManager::create_instance();
        entityManager->init();
        playerManager = Player::PlayerManager::create_instance();
        playerManager->init();
        deltatime = Core::DeltaTime::create_instance();
        deltatime->init();
        assetManager = Asset::AssetManager::create_instance();
        assetManager->init();
        physicsManager = Physics::PhysicsManager::create_instance();
        physicsManager->init();

        auto* boxModel = Tools::import_model("../assets/Box.glb");
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> ydistr(0, 75.0);
        std::uniform_real_distribution<float> xzdistr(-25.0, 25.0);
        std::uniform_real_distribution<float> rotdistr(-360, 360);
        for(size_t i = 0; i < 500; i++) {
            Entity::Entity *box = entityManager->create_entity();
            box->add_component<Component::Model>();
            box->get_component<Component::Model>()->set_asset(boxModel->get_id());
            box->get_component<Component::Transform>()->position[0] = xzdistr(gen);
            box->get_component<Component::Transform>()->position[1] = ydistr(gen);
            box->get_component<Component::Transform>()->position[2] = xzdistr(gen);
            //box->get_component<Component::Transform>()->rotation[0] = rotdistr(gen);
            //box->get_component<Component::Transform>()->rotation[1] = rotdistr(gen);
            //box->get_component<Component::Transform>()->rotation[2] = rotdistr(gen);
            //box->get_component<Component::Transform>()->scale[0] = 2.0f;
            //box->get_component<Component::Transform>()->scale[1] = 2.0f;
            //box->get_component<Component::Transform>()->scale[2] = 2.0f;
            box->set_name(boxModel->get_name());
            box->add_component<Component::RigidbodyDynamic>();
        }

        auto* sponzaModel = Tools::import_model("../assets/sponza-gltf-pbr/sponza.glb");
        Entity::Entity* sponza = entityManager->create_entity();
        sponza->add_component<Component::Model>();
        sponza->get_component<Component::Model>()->set_asset(sponzaModel->get_id());
        sponza->set_name(sponzaModel->get_name());
        sponza->get_component<Component::Transform>()->scale[0] = 0.1f;
        sponza->get_component<Component::Transform>()->scale[1] = 0.1f;
        sponza->get_component<Component::Transform>()->scale[2] = 0.1f;
        //sponza->add_component<Component::RigidbodyStatic>();
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

            // tick subsystems
            physicsManager->tick();
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