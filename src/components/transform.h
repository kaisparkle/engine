#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <components/component.h>

namespace Component {
    class Transform : public IComponent {
    public:
        Transform(Entity::Entity* entity);

        // returns a translation-rotation-scale matrix
        glm::mat4 get_matrix();

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
    };
}