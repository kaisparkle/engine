#pragma once

#include <string>
#include <queue>
#include <map>
#include <entities/entity.h>

// TODO: cvars?
#define MAX_ENTITIES 5000

namespace Entity {
    class EntityManager {
    public:
        // create the EntityManager singleton
        static EntityManager* create_instance();
        // destroy the EntityManager singleton
        static void destroy_instance();
        // return the EntityManager singleton
        static EntityManager* get_instance();

        // initialize the Entity Manager
        void init();
        // clean up the Entity Manager
        void cleanup();

        // create a new entity - returns a pointer to it
        Entity* create_entity();
        // destroy the entity with the given ID
        void destroy_entity(uint32_t entityId);
        // returns a pointer to the entity with given ID
        Entity* get_entity(uint32_t entityId);
    private:
        inline static EntityManager* instance;

        std::queue<uint32_t> availableIds;
        uint32_t entityCount;
        // TODO: is a map here a good idea?
        std::map<uint32_t, Entity> entities;
    };
}