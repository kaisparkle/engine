#include <cassert>
#include <optick.h>
#include <component/transform.h>
#include <entity/entitymanager.h>

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
        OPTICK_EVENT();
        assert(entityCount < MAX_ENTITIES);
        uint32_t id = availableIds.front();
        availableIds.pop();
        entityCount++;

        entities.emplace(id, id);

        // add a transform
        entities.at(id).add_component<Component::Transform>();

        return &entities.at(id);
    }

    void EntityManager::destroy_entity(uint32_t entityId) {
        OPTICK_EVENT();
        entities.erase(entityId);
        availableIds.push(entityId);
        entityCount--;
    }

    Entity* EntityManager::get_entity(uint32_t entityId) {
        OPTICK_EVENT();
        Entity* result = nullptr;
        result = &entities.at(entityId);
        return result;
    }

    std::vector<uint32_t> EntityManager::get_active_ids() {
        // TODO: this is probably piss slow
        OPTICK_EVENT();
        std::vector<uint32_t> ids;
        for(auto &entity : entities) {
            ids.push_back(entity.first);
        }
        return ids;
    }
}