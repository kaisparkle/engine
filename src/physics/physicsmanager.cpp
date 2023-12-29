#include <cassert>
#include <iostream>
#include <component/transform.h>
#include <physics/internal.h>
#include <optick.h>
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
    }

    void PhysicsManager::cleanup() {
        JPH::UnregisterTypes();
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsManager::tick() {
        OPTICK_EVENT();
        if(!physicsEnabled) return;

        const float deltaTime = 1.0f / 60.0f;
        const uint collisionSteps = 1;
        physicsSystem.Update(deltaTime, collisionSteps, tempAllocator, jobSystem);

        for(auto& body : bodies) {
            if(physicsSystem.GetBodyInterface().GetMotionType(body.second) != JPH::EMotionType::Static) {
                auto* transform = body.first->get_parent()->get_parent()->get_component<Component::Transform>();
                JPH::Vec3 pos = physicsSystem.GetBodyInterface().GetPosition(body.second);
                JPH::Quat rot = physicsSystem.GetBodyInterface().GetRotation(body.second);
                transform->position.x = pos.GetX();
                transform->position.y = pos.GetY();
                transform->position.z = pos.GetZ();
                transform->rotation = glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
            }
        }
    }

    void PhysicsManager::register_collider(Collider::ICollider* collider, bool isDynamic) {
        if(bodies.find(collider) != bodies.end()) {
            physicsSystem.GetBodyInterface().RemoveBody(bodies.at(collider));
        }

        JPH::ShapeSettings::ShapeResult shapeResult = collider->get_shape_settings()->Create();
        if (shapeResult.HasError()) {
            std::cout << shapeResult.GetError() << std::endl;
            return;
        } else {
            std::cout << "Created shape" << std::endl;
        }

        auto* transform = collider->get_parent()->get_parent()->get_component<Component::Transform>();
        JPH::Vec3 position = {transform->position.x, transform->position.y, transform->position.z};
        JPH::Quat rotation = JPH::Quat(transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w);
        JPH::Vec3 scale = {transform->scale.x, transform->scale.y, transform->scale.z};

        JPH::ShapeRefC shape = shapeResult.Get()->ScaleShape(scale).Get();

        JPH::Body *body;
        JPH::BodyCreationSettings* bodySettings;
        if(isDynamic) {
            bodySettings = new JPH::BodyCreationSettings(shape, position, rotation, JPH::EMotionType::Dynamic, Layers::MOVING);
            bodySettings->mRestitution = 0.5f;
            body = physicsSystem.GetBodyInterface().CreateBody(*bodySettings);
            physicsSystem.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::Activate);
        } else {
            bodySettings = new JPH::BodyCreationSettings(shape, position, rotation, JPH::EMotionType::Static, Layers::NON_MOVING);
            bodySettings->mFriction = 1.0f;
            body = physicsSystem.GetBodyInterface().CreateBody(*bodySettings);
            physicsSystem.GetBodyInterface().AddBody(body->GetID(), JPH::EActivation::DontActivate);
        }

        bodies.emplace(collider, body->GetID());
        delete bodySettings;
    }

    void PhysicsManager::update_position(Collider::ICollider* collider) {
        auto* transform = collider->get_parent()->get_parent()->get_component<Component::Transform>();
        JPH::BodyID body = bodies.at(collider);
        JPH::Vec3 pos = {transform->position.x, transform->position.y, transform->position.z};
        physicsSystem.GetBodyInterface().SetPosition(body, pos, JPH::EActivation::Activate);
        activate_all();
    }

    void PhysicsManager::update_rotation(Collider::ICollider* collider) {
        auto* transform = collider->get_parent()->get_parent()->get_component<Component::Transform>();
        JPH::BodyID body = bodies.at(collider);
        JPH::Quat rot = JPH::Quat(transform->rotation.x, transform->rotation.y, transform->rotation.z, transform->rotation.w);
        physicsSystem.GetBodyInterface().SetRotation(body, rot, JPH::EActivation::Activate);
        activate_all();
    }

    void PhysicsManager::update_scale(Collider::ICollider* collider) {
        auto* transform = collider->get_parent()->get_parent()->get_component<Component::Transform>();
        JPH::BodyID body = bodies.at(collider);
        JPH::Vec3 scale = {transform->scale.x, transform->scale.y, transform->scale.z};
        JPH::ShapeRefC scaledShape = physicsSystem.GetBodyInterface().GetShape(body)->ScaleShape(scale).Get();
        physicsSystem.GetBodyInterface().SetShape(body, scaledShape, false, JPH::EActivation::Activate);
        activate_all();
    }

    void PhysicsManager::activate_all() {
        for(auto& body : bodies) {
            physicsSystem.GetBodyInterface().ActivateBody(body.second);
        }
    }

    void PhysicsManager::set_enabled(bool enabled) {
        physicsEnabled = enabled;
    }
}