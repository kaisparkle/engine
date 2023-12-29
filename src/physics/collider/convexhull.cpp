#include <physics/collider/convexhull.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>

namespace Physics::Collider {
    ConvexHullCollider::ConvexHullCollider(Component::Rigidbody* colliderParent, Core::Mesh* mesh) : ICollider(colliderParent) {
        JPH::Array<JPH::Vec3> vertices;
        vertices.resize(mesh->vertices.size());
        for(size_t i = 0; i < mesh->vertices.size(); i++) {
            JPH::Vec3 v = {mesh->vertices[i].position[0], mesh->vertices[i].position[1], mesh->vertices[i].position[2]};
            vertices[i] = v;
        }

        JPH::ShapeSettings* settings = new JPH::ConvexHullShapeSettings(vertices, JPH::cDefaultConvexRadius);
        set_shape_settings(settings);
    }
}