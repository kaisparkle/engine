#include <cassert>
#include <random>
#include <optick.h>
#include <render/gl/renderer.h>
#include <core/window.h>
#include <ui/editor.h>
#include <tools/modelimport.h>
#include <component/rigidbody.h>
#include <physics/collider/convexhull.h>
#include <physics/collider/box.h>
#include <physics/collider/mesh.h>
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
        physicsManager = Physics::PhysicsManager::create_instance();
        physicsManager->init();
        deltatime = Core::DeltaTime::create_instance();
        deltatime->init();
        assetManager = Asset::AssetManager::create_instance();
        assetManager->init();
        playerManager = Player::PlayerManager::create_instance();
        playerManager->init();

        Entity::Entity* floor = entityManager->create_entity();
        floor->set_name("Floor");
        floor->get_component<Component::Transform>()->position.y = -1.25f;
        auto* floorBody = floor->add_component<Component::Rigidbody>();
        auto* floorCollider = new Physics::Collider::BoxCollider(floorBody, glm::vec3(1000.0f, 1.0f, 1000.0f));
        floorBody->add_collider(floorCollider);
        floorBody->initialize();

        auto* sponzaModel = Tools::import_model("../assets/sponza-gltf-pbr/sponza.glb");
        Entity::Entity* sponza = entityManager->create_entity();
        sponza->add_component<Component::Model>();
        sponza->get_component<Component::Model>()->set_asset(sponzaModel->get_id());
        sponza->set_name(sponzaModel->get_name());
        sponza->get_component<Component::Transform>()->scale = glm::vec3(0.05f, 0.05f, 0.05f);
        auto* sponzaBody = sponza->add_component<Component::Rigidbody>();
        for(auto mesh : sponzaModel->meshes) {
            auto* collider = new Physics::Collider::MeshCollider(sponzaBody, mesh);
            sponzaBody->add_collider(collider);
        }
        sponzaBody->initialize();

        auto* boxModel = Tools::import_model("../assets/Box.glb");
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> ydistr(1, 30);
        std::uniform_real_distribution<float> xzdistr(-25.0, 25.0);
        std::uniform_real_distribution<float> rotdistr(-360, 360);
        for(size_t i = 0; i < 250; i++) {
            Entity::Entity *box = entityManager->create_entity();
            box->add_component<Component::Model>();
            box->get_component<Component::Model>()->set_asset(boxModel->get_id());
            box->get_component<Component::Transform>()->position = glm::vec3(xzdistr(gen), ydistr(gen), xzdistr(gen));
            box->get_component<Component::Transform>()->rotation = glm::quat(glm::radians(glm::vec3(rotdistr(gen), rotdistr(gen), rotdistr(gen))));
            box->set_name(boxModel->get_name());
            auto* body = box->add_component<Component::Rigidbody>();
            for(auto mesh : boxModel->meshes) {
                auto* collider = new Physics::Collider::ConvexHullCollider(body, mesh);
                body->add_collider(collider);
            }
            body->set_dynamic(true);
            body->initialize();
        }
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