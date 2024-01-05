#pragma once

#include <entity/entity.h>

namespace Component {
    class IComponent {
    public:
        IComponent(Entity::Entity* entity) {parent = entity;}
        virtual ~IComponent() = default;

        Entity::Entity* get_parent() {return parent;}
    private:
        Entity::Entity* parent;
    };
}