#pragma once

#include <core/window.h>
#include <core/deltatime.h>
#include <ui/editor.h>
#include <render/renderer.h>
#include <entities/entitymanager.h>
#include <player/playermanager.h>

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
        UI::Editor* editor;
        Render::IRenderer* renderer;
        Entity::EntityManager* entityManager;
        Player::PlayerManager* playerManager;
        Core::DeltaTime* deltatime;
    };
}