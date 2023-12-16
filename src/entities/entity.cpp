#include <entities/entity.h>

namespace Entity {
    Entity::Entity(uint32_t entityId) {
        id = entityId;
    }

    Entity::~Entity() {
        for(Component::IComponent* component : components) {
            delete component;
        }
        components.clear();
    }

    uint32_t Entity::get_id() {
        OPTICK_EVENT();
        return id;
    }
}