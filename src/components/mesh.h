#pragma once

#include <vector>
#include <components/component.h>
#include <core/vertex.h>

namespace Component {
    class Mesh : public IComponent {
    public:
        Mesh(Entity::Entity* entity) : IComponent(entity) {}

        std::vector<Core::Vertex> vertices;
        std::vector<unsigned int> indices;
        void* api_objects;
    };
};