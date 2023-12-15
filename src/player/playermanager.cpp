#include <cassert>
#include <entities/entitymanager.h>
#include <components/camera.h>
#include <core/window.h>
#include <player/playermanager.h>

namespace Player {
    PlayerManager* PlayerManager::create_instance() {
        assert(instance == nullptr);
        instance = new PlayerManager;
        return instance;
    }

    void PlayerManager::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    PlayerManager* PlayerManager::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void PlayerManager::init() {
        // create an entity for the player
        entityId = Entity::EntityManager::get_instance()->create_entity();

        // slap a camera on it
        Entity::EntityManager::get_instance()->get_entity(entityId)->add_component<Component::Camera>();

        // set up the camera
        // TODO: cvars for defaults
        float aspect = Core::Window::get_instance()->get_window_width() / Core::Window::get_instance()->get_window_height();
        Entity::EntityManager::get_instance()->get_entity(entityId)->get_component<Component::Camera>()
                ->update_projection(90.0f,aspect, 0.1f, 2000.0f);
    }

    void PlayerManager::cleanup() {
        // do cleanup things
    }

    uint32_t PlayerManager::get_entity_id() {
        return entityId;
    }
}