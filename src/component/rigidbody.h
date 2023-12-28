#pragma once

#include <component/component.h>

namespace Component {
    class RigidbodyStatic : public IComponent {
    public:
        RigidbodyStatic(Entity::Entity* entity);
    };

    class RigidbodyDynamic : public IComponent {
    public:
        RigidbodyDynamic(Entity::Entity* entity);
    };
}