#include <physics/physicsmanager.h>
#include <component/rigidbody.h>

namespace Component {
    void Rigidbody::initialize() {
        for(auto collider : colliders) {
            Physics::PhysicsManager::get_instance()->register_collider(collider, isDynamic);
        }
    }

    void Rigidbody::add_collider(Physics::Collider::ICollider *collider) {
        colliders.push_back(collider);
    }

    void Rigidbody::update_position() {
        for(auto collider : colliders) {
            Physics::PhysicsManager::get_instance()->update_position(collider);
        }
    }

    void Rigidbody::update_rotation() {
        for(auto collider : colliders) {
            Physics::PhysicsManager::get_instance()->update_rotation(collider);
        }
    }

    void Rigidbody::update_scale() {
        for(auto collider : colliders) {
            Physics::PhysicsManager::get_instance()->update_scale(collider);
        }
    }

    void Rigidbody::set_dynamic(bool dynamic) {
        isDynamic = dynamic;
    }
}