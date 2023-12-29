#include <cassert>
#include <optick.h>
#include <entity/entitymanager.h>
#include <component/camera.h>
#include <component/flycontroller.h>
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
        entityId = Entity::EntityManager::get_instance()->create_entity()->get_id();
        Entity::EntityManager::get_instance()->get_entity(entityId)->set_name("Player");

        // slap a camera and controller on it
        Entity::EntityManager::get_instance()->get_entity(entityId)->add_component<Component::Camera>();
        Entity::EntityManager::get_instance()->get_entity(entityId)->add_component<Component::FlyController>();

        // set up the camera
        // TODO: cvars for defaults
        float aspect = (float)Core::Window::get_instance()->get_window_width() / (float)Core::Window::get_instance()->get_window_height();
        get_player_camera()->update_projection(90.0f,aspect, 0.1f, 10000.0f);

        get_player_transform()->position[0] = -50.0f;
        get_player_transform()->position[1] = 10.0f;

        get_player_controller()->process_mouse(0.0f, 0.0f);
    }

    void PlayerManager::cleanup() {
        // do cleanup things
    }

    uint32_t PlayerManager::get_entity_id() {
        OPTICK_EVENT();
        return entityId;
    }

    Component::Camera* PlayerManager::get_player_camera() {
        OPTICK_EVENT();
        return Entity::EntityManager::get_instance()->get_entity(entityId)->get_component<Component::Camera>();
    }

    Component::Transform *PlayerManager::get_player_transform() {
        OPTICK_EVENT();
        return Entity::EntityManager::get_instance()->get_entity(entityId)->get_component<Component::Transform>();
    }

    Component::IController *PlayerManager::get_player_controller() {
        OPTICK_EVENT();
        return Entity::EntityManager::get_instance()->get_entity(entityId)->get_component<Component::IController>();
    }
}