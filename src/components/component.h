#pragma once

// forward declaration
namespace Entity {
    class Entity;
}

namespace Component {
    class IComponent {
    public:
        IComponent(Entity::Entity* entity) {parent = entity;}
        virtual ~IComponent() = default;
    private:
        Entity::Entity* parent;
    };
}