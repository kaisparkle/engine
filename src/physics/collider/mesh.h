#pragma once

#include <core/mesh.h>
#include <physics/collider/collider.h>

namespace Physics::Collider {
    class MeshCollider : public ICollider {
    public:
        MeshCollider(Component::Rigidbody* colliderParent, Core::Mesh* mesh);
    };
}