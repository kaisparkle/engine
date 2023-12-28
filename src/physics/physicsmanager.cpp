#include <cassert>
#include <iostream>
#include <physics/internal.h>
#include <Jolt/Physics/Collision/Shape/MeshShape.h>
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <glm/glm.hpp>
#include <physics/physicsmanager.h>

namespace Physics {
    PhysicsManager* PhysicsManager::create_instance() {
        assert(instance == nullptr);
        instance = new PhysicsManager;
        return instance;
    }

    void PhysicsManager::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    PhysicsManager* PhysicsManager::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void PhysicsManager::init() {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();
        tempAllocator = new JPH::TempAllocatorMalloc();
        jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, JPH::thread::hardware_concurrency() - 1);

        const uint maxBodies = 65535;
        const uint numBodyMutexes = 0;
        const uint maxBodyPairs = 65535;
        const uint maxContactConstraints = 65535;

        broadPhaseLayerInterface = new BroadPhaseLayerInterfaceImpl();
        objectVsBroadPhaseLayerFilter = new ObjectVsBroadPhaseLayerFilterImpl();
        objectVsObjectLayerFilter = new ObjectLayerPairFilterImpl();

        physicsSystem.Init(maxBodies, numBodyMutexes, maxBodyPairs, maxContactConstraints,
                           *broadPhaseLayerInterface, *objectVsBroadPhaseLayerFilter, *objectVsObjectLayerFilter);

        JPH::BoxShapeSettings floorSettings(JPH::Vec3(1000.0f, 1.0f, 1000.0f));
        JPH::ShapeRefC floorShape = floorSettings.Create().Get();
        JPH::BodyCreationSettings bodySettings(floorShape, JPH::RVec3(0.0f, -1.0f, 0.0f), JPH::Quat::sIdentity(), JPH::EMotionType::Static, Layers::NON_MOVING);
        JPH::Body* floor = physicsSystem.GetBodyInterface().CreateBody(bodySettings);
        physicsSystem.GetBodyInterface().AddBody(floor->GetID(), JPH::EActivation::DontActivate);
    }

    void PhysicsManager::cleanup() {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsManager::tick() {
        const float deltaTime = 1.0f / 60.0f;
        const uint collisionSteps = 1;
        physicsSystem.Update(deltaTime, collisionSteps, tempAllocator, jobSystem);

        for(auto& body : bodies) {
            if(physicsSystem.GetBodyInterface().GetMotionType(body.second) != JPH::EMotionType::Static) {
                JPH::Vec3 pos = physicsSystem.GetBodyInterface().GetPosition(body.second);
                JPH::Vec3 rot = physicsSystem.GetBodyInterface().GetRotation(body.second).GetEulerAngles();
                body.first->position[0] = pos.GetX();
                body.first->position[1] = pos.GetY();
                body.first->position[2] = pos.GetZ();
                body.first->rotation[0] = glm::degrees(rot.GetX());
                body.first->rotation[1] = glm::degrees(rot.GetY());
                body.first->rotation[2] = glm::degrees(rot.GetZ());
            }
        }
    }

    void PhysicsManager::register_mesh(Component::Transform* transform, Core::Mesh* mesh, const std::string& type) {
        JPH::ShapeSettings* shapeSettings;
        if(type == "static") {
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

            shapeSettings = new JPH::MeshShapeSettings(vertexList, triangleList);
        } else {
            JPH::Array<JPH::Vec3> vertices;
            vertices.resize(mesh->vertices.size());
            for(size_t i = 0; i < mesh->vertices.size(); i++) {
                JPH::Vec3 v = {mesh->vertices[i].position[0], mesh->vertices[i].position[1], mesh->vertices[i].position[2]};
                vertices[i] = v;
            }

            shapeSettings = new JPH::ConvexHullShapeSettings(vertices, JPH::cDefaultConvexRadius);
        }
        JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings->Create();
        if (shapeResult.HasError()) {
            std::cout << shapeResult.GetError() << std::endl;
            return;
        } else {
            std::cout << "Created shape" << std::endl;
        }
        JPH::ShapeRefC shape = shapeResult.Get()->ScaleShape(JPH::Vec3(transform->scale[0], transform->scale[1], transform->scale[2])).Get();

        JPH::Vec3 pos = {transform->position[0], transform->position[1], transform->position[2]};
        JPH::Quat rot = JPH::Quat::sEulerAngles(JPH::Vec3(
                glm::radians(transform->rotation[0]),
                glm::radians(transform->rotation[1]),
                glm::radians(transform->rotation[2])));
        JPH::Body *body;
        JPH::BodyCreationSettings* bodySettings;
        if(type == "static") {
            bodySettings = new JPH::BodyCreationSettings(shape, pos, rot, JPH::EMotionType::Static, Layers::NON_MOVING);
            body = physicsSystem.GetBodyInterface().CreateBody(*bodySettings);
            physicsSystem.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::DontActivate);
        } else {
            bodySettings = new JPH::BodyCreationSettings(shape, pos, rot, JPH::EMotionType::Dynamic, Layers::MOVING);
            body = physicsSystem.GetBodyInterface().CreateBody(*bodySettings);
            physicsSystem.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
        }

        bodies.emplace(transform, body->GetID());

        free(bodySettings);
        free(shapeSettings);
    }

    void PhysicsManager::update_position(Component::Transform *transform) {
        JPH::BodyID body = bodies.at(transform);
        JPH::Vec3 pos = {transform->position[0], transform->position[1], transform->position[2]};
        physicsSystem.GetBodyInterface().SetPosition(body, pos, JPH::EActivation::Activate);
    }

    void PhysicsManager::update_rotation(Component::Transform *transform) {
        JPH::BodyID body = bodies.at(transform);
        JPH::Quat rot = JPH::Quat::sEulerAngles(JPH::Vec3(
                glm::radians(transform->rotation[0]),
                glm::radians(transform->rotation[1]),
                glm::radians(transform->rotation[2])));
        physicsSystem.GetBodyInterface().SetRotation(body, rot, JPH::EActivation::Activate);
    }

    void PhysicsManager::update_scale(Component::Transform *transform) {
        JPH::BodyID body = bodies.at(transform);
        JPH::Vec3 scale = {transform->scale[0], transform->scale[1], transform->scale[2]};
        JPH::ShapeRefC scaledShape = physicsSystem.GetBodyInterface().GetShape(body)->ScaleShape(scale).Get();
        physicsSystem.GetBodyInterface().SetShape(body, scaledShape, false, JPH::EActivation::Activate);
    }
}