#pragma once

#include <map>
#include <physics/collider/collider.h>
#include <physics/internal.h>
#include <Jolt/Physics/Character/Character.h>

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

        // tick over the simulation state
        void tick();

        // register a new collider with the PhysicsManager, or replace an existing one
        void register_collider(Collider::ICollider* collider, bool isDynamic);

        // update the position in the simulation with the position in the parent transform
        void update_position(Collider::ICollider* collider);
        // update the rotation in the simulation with the rotation in the parent transform
        void update_rotation(Collider::ICollider* collider);
        // update the scale in the simulation with the scale in the parent transform
        void update_scale(Collider::ICollider* collider);

        // activate all bodies in the simulation
        void activate_all();

        // create a character
        JPH::Character* create_character(JPH::CharacterSettings* settings, Collider::ICollider* collider, JPH::Vec3 position, JPH::Quat rotation);

        void set_enabled(bool enabled);
    private:
        inline static PhysicsManager* instance;

        JPH::TempAllocatorMalloc* tempAllocator;
        JPH::JobSystemThreadPool* jobSystem;
        JPH::PhysicsSystem physicsSystem;

        BroadPhaseLayerInterfaceImpl* broadPhaseLayerInterface;
        ObjectVsBroadPhaseLayerFilterImpl* objectVsBroadPhaseLayerFilter;
        ObjectLayerPairFilterImpl* objectVsObjectLayerFilter;

        bool physicsEnabled = false;

        std::map<Collider::ICollider*, JPH::BodyID> bodies;
        std::vector<JPH::Character*> characters;
    };
}