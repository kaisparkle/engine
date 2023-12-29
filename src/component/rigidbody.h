#pragma once

#include <component/component.h>

// forward declaration
namespace Physics::Collider {
    class ICollider;
}

namespace Component {
    class Rigidbody : public IComponent {
    public:
        Rigidbody(Entity::Entity* entity) : IComponent(entity) {};

        // initialize the Rigidbody - must be called again after modification
        void initialize();

        // add a collider to the Rigidbody
        void add_collider(Physics::Collider::ICollider* collider);

        // update the position from the parent transform
        void update_position();
        // update the rotation from the parent transform
        void update_rotation();
        // update the scale from the parent transform
        void update_scale();

        // set whether the body is dynamic
        void set_dynamic(bool dynamic);
    private:
        std::vector<Physics::Collider::ICollider*> colliders;

        bool isDynamic = false;
    };
}