#pragma once

#include <glm/glm.hpp>
#include <components/component.h>

namespace Component {
    class Camera : public IComponent {
    public:
        Camera(Entity::Entity* entity) : IComponent(entity) {}

        void update_projection(float fovDeg, float aspectRatio, float nearPlane, float farPlane);
        glm::mat4 get_matrix();
    private:
        glm::mat4 projection;
        float fov;
        float aspect;
        float near;
        float far;
    };
}