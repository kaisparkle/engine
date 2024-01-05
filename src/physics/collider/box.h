#pragma once

#include <glm/glm.hpp>
#include <physics/collider/collider.h>

namespace Physics::Collider {
    class BoxCollider : public ICollider {
    public:
        BoxCollider(Component::Rigidbody* colliderParent, glm::vec3 size);
    };
}