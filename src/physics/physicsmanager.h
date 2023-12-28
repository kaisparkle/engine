#pragma once

#include <map>
#include <component/transform.h>
#include <component/model.h>
#include <physics/internal.h>

namespace Physics {
    class PhysicsManager {
    public:
        // create the PhysicsManager singleton
        static PhysicsManager* create_instance();
        // destroy the PhysicsManager singleton
        static void destroy_instance();
        // return the PhysicsManager singleton
        static PhysicsManager* get_instance();

        // initialize the PhysicsManager
        void init();
        // clean up the PhysicsManager
        void cleanup();

        void tick();

        void register_mesh(Component::Transform* transform, Core::Mesh* mesh, const std::string& type);

        void update_position(Component::Transform* transform);
        void update_rotation(Component::Transform* transform);
        void update_scale(Component::Transform* transform);
    private:
        inline static PhysicsManager* instance;

        JPH::TempAllocatorMalloc* tempAllocator;
        JPH::JobSystemThreadPool* jobSystem;
        JPH::PhysicsSystem physicsSystem;

        BroadPhaseLayerInterfaceImpl* broadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl* objectVsBroadPhaseLayerFilter;
        ObjectLayerPairFilterImpl* objectVsObjectLayerFilter;

        std::map<Component::Transform*, JPH::BodyID> bodies;
    };
}