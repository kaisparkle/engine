#pragma once

#include <physics/collider/collider.h>

namespace Physics::Collider {
    class CapsuleCollider : public ICollider {
    public:
        CapsuleCollider(Component::Rigidbody* colliderParent, float height, float radius);
    };
}