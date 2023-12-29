#pragma once

#include <core/mesh.h>
#include <physics/collider/collider.h>

namespace Physics::Collider {
    class ConvexHullCollider : public ICollider {
    public:
        ConvexHullCollider(Component::Rigidbody* colliderParent, Core::Mesh* mesh);
    };
}