#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <optick.h>
#include <components/component.h>

namespace Entity {
    class Entity {
    public:
        Entity(uint32_t entityId);
        ~Entity();

        // returns this entity's ID
        uint32_t get_id();

        // add a new component of the given type
        template<typename ComponentType>
        void add_component() {
            OPTICK_EVENT();
            components.push_back(new ComponentType(this));
        }

        // destroy component of given type
        template<typename ComponentType>
        void destroy_component() {
            OPTICK_EVENT();
            ComponentType* component = get_component<ComponentType>();

            auto idx = std::find(components.begin(), components.end(), component);
            if(idx != components.end()) {
                components.erase(idx);
            }

            delete component;
        }

        // returns a pointer to component of given type, if it exists
        template<typename ComponentType>
        ComponentType* get_component() {
            OPTICK_EVENT();
            ComponentType* result = nullptr;
            for(Component::IComponent* component : components) {
                // if this dynamic cast succeeds, we found it
                result = dynamic_cast<ComponentType*>(component);
                if(result) break;
            }

            return result;
        }
    private:
        // TODO: is a vector here a good idea?
        std::vector<Component::IComponent*> components;
        uint32_t id;
    };
}