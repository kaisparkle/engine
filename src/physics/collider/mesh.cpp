#include <physics/collider/mesh.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>

namespace Physics::Collider {
    MeshCollider::MeshCollider(Component::Rigidbody* colliderParent, Core::Mesh* mesh) : ICollider(colliderParent) {
        JPH::VertexList vertexList;
        JPH::IndexedTriangleList triangleList;

        vertexList.resize(mesh->vertices.size());
        for (size_t i = 0; i < mesh->vertices.size(); i++) {
            JPH::Float3 pos = {mesh->vertices[i].position.x, mesh->vertices[i].position.y,
                               mesh->vertices[i].position.z};
            vertexList[i] = pos;
        }

        triangleList.resize(mesh->vertices.size() / 3);
        uint32_t indexCounter = 0;
        uint32_t triangleCounter = 0;
        while (indexCounter < mesh->indices.size() && triangleCounter < mesh->vertices.size() / 3) {
            JPH::IndexedTriangle triangle(mesh->indices[indexCounter], mesh->indices[indexCounter + 1],
                                          mesh->indices[indexCounter + 2]);
            indexCounter += 3;
            triangleList[triangleCounter] = triangle;
            triangleCounter++;
        }

        JPH::ShapeSettings* settings = new JPH::MeshShapeSettings(vertexList, triangleList);
        set_shape_settings(settings);
    }
}