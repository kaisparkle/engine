#pragma once

#include <cstdint>
#include <components/camera.h>
#include <components/transform.h>
#include <components/controller.h>

namespace Player {
    class PlayerManager {
    public:
        // create the PlayerManager singleton
        static PlayerManager* create_instance();
        // destroy the PlayerManager singleton
        static void destroy_instance();
        // return the PlayerManager singleton
        static PlayerManager* get_instance();

        // initialize the PlayerManager
        void init();
        // clean up the PlayerManager
        void cleanup();

        // get the player entity id
        uint32_t get_entity_id();
        // get the player camera by pointer
        Component::Camera* get_player_camera();
        // get the player transform by pointer
        Component::Transform* get_player_transform();
        // get the player controller by pointer
        Component::IController* get_player_controller();
    private:
        inline static PlayerManager* instance;

        uint32_t entityId;
    };
}