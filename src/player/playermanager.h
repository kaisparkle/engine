#pragma once

#include <stdint.h>

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
    private:
        inline static PlayerManager* instance;

        uint32_t entityId;
    };
}