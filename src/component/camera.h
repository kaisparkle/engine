#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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
        // get the orientation quat
        glm::quat get_orientation();
        // offset camera angles
        void offset_angles(float up, float right);
        // update the camera vectors
        void update_vectors();
    private:
        glm::mat4 projection;
        float fov;
        float aspect;
        float near;
        float far;
        glm::quat orientation;
        float upAngle;
        float rightAngle;
    };
}