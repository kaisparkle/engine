#pragma once

#include <glm/glm.hpp>
#include <components/component.h>

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
        // take in mouse input
        void process_mouse(float dx, float dy);
        void process_keyboard(double delta, const uint8_t *keystate);
    private:
        glm::mat4 projection;
        float fov;
        float aspect;
        float near;
        float far;
        glm::vec3 up;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 worldUp;
        float pitch;
        float yaw;
        float sensitivity;
        float velocity;

        void update_vectors();
    };
}