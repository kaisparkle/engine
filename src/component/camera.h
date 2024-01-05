#pragma once

#include <glm/glm.hpp>
#include <component/component.h>

namespace Component {
    class Camera : public IComponent {
    public:
        Camera(Entity::Entity* entity);

        // update the camera projection
        void update_projection(float fovDeg, float aspectRatio, float nearPlane, float farPlane);
        // get the untranslated view matrix
        glm::mat4 get_view_matrix();
        // get the projection matrix
        glm::mat4 get_projection_matrix();
        // update the camera vectors
        void update_vectors(float pitch, float yaw);
        // get the front vector - includes pitch component
        glm::vec3 get_front();
        // get the forward vector - excludes pitch component
        glm::vec3 get_forward();
        // get the right vector
        glm::vec3 get_right();
        // get the up vector
        glm::vec3 get_up();
    private:
        glm::mat4 projection;
        float fov;
        float aspect;
        float near;
        float far;
        glm::vec3 worldUp;
        glm::vec3 up;
        glm::vec3 front;
        glm::vec3 forward;
        glm::vec3 right;
    };
}