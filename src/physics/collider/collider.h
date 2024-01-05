#pragma once

#include <component/rigidbody.h>
#include <physics/internal.h>

namespace Physics::Collider {
    class ICollider {
    public:
        ICollider(Component::Rigidbody* colliderParent) {
            shapeSettings = nullptr;
            parent = colliderParent;
        };

        ~ICollider() {delete shapeSettings;};

        JPH::ShapeSettings* get_shape_settings() {return shapeSettings;}
        void set_shape_settings(JPH::ShapeSettings* settings) {shapeSettings = settings;}

        Component::Rigidbody* get_parent() {return parent;}
    private:
        JPH::ShapeSettings* shapeSettings;
        Component::Rigidbody* parent;
    };
}