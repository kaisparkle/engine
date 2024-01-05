#include <physics/collider/box.h>

namespace Physics::Collider {
    BoxCollider::BoxCollider(Component::Rigidbody* colliderParent, glm::vec3 size) : ICollider(colliderParent) {
        JPH::ShapeSettings* settings = new JPH::BoxShapeSettings(JPH::Vec3(size.x, size.y, size.z));
        set_shape_settings(settings);
    }
}