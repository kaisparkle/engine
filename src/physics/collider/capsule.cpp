#include <physics/collider/capsule.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>

namespace Physics::Collider {
    CapsuleCollider::CapsuleCollider(Component::Rigidbody* colliderParent, float height, float radius) : ICollider(colliderParent) {
        JPH::ShapeSettings* settings = new JPH::CapsuleShapeSettings(0.5f * height, radius);
        set_shape_settings(settings);
    }
}