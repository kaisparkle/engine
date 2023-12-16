#pragma once

#include <string>
#include <queue>
#include <map>
#include <entities/entity.h>

// TODO: cvars?
#define MAX_ENTITIES 16384

namespace Entity {
    class EntityManager {
    public:
        // create the EntityManager singleton
        static EntityManager* create_instance();
        // destroy the EntityManager singleton
        static void destroy_instance();
        // return the EntityManager singleton
        static EntityManager* get_instance();

        // initialize the EntityManager
        void init();
        // clean up the EntityManager
        void cleanup();

        // create a new entity - returns its ID
        uint32_t create_entity();
        // destroy the entity with the given ID
        void destroy_entity(uint32_t entityId);
        // returns a pointer to the entity with given ID
        Entity* get_entity(uint32_t entityId);
        // returns a vector of all active IDs
        std::vector<uint32_t> get_active_ids();
    private:
        inline static EntityManager* instance;

        std::queue<uint32_t> availableIds;
        uint32_t entityCount;
        // TODO: is a map here a good idea?
        std::map<uint32_t, Entity> entities;
    };
}