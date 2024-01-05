#include <component/component.h>
#include <entity/entity.h>

namespace Entity {
    Entity::Entity(uint32_t entityId) {
        id = entityId;
        name = "";
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

    const std::string& Entity::get_name() {
        return name;
    }

    void Entity::set_name(const std::string &newName) {
        name = newName;
    }
}