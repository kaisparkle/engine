#include <cassert>
#include <entities/entitymanager.h>

namespace Entity {
    EntityManager* EntityManager::create_instance() {
        assert(instance == nullptr);
        instance = new EntityManager;
        return instance;
    }

    void EntityManager::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    EntityManager* EntityManager::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void EntityManager::init() {
        for(uint32_t id = 0; id < MAX_ENTITIES; id++) {
            availableIds.push(id);
        }

        entityCount = 0;
    }

    void EntityManager::cleanup() {
        entities.clear();
    }

    Entity* EntityManager::create_entity() {
        assert(entityCount < MAX_ENTITIES);
        uint32_t id = availableIds.front();
        availableIds.pop();
        entityCount++;

        entities.emplace(id, id);

        return &entities.at(id);
    }

    void EntityManager::destroy_entity(uint32_t entityId) {
        entities.erase(entityId);
        availableIds.push(entityId);
        entityCount--;
    }

    Entity* EntityManager::get_entity(uint32_t entityId) {
        Entity* result = nullptr;
        result = &entities.at(entityId);
        return result;
    }
}