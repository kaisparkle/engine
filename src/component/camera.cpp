#include <optick.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <player/playermanager.h>
#include <component/camera.h>

namespace Component {
    Camera::Camera(Entity::Entity *entity) : IComponent(entity) {
        update_projection(90.0f, (float)1600/(float)900, 0.1f, 10000.0f);
        upAngle = 0.0f;
        rightAngle = 90.0f;
        update_vectors();
    }

    void Camera::update_projection(float fovDeg, float aspectRatio, float nearPlane, float farPlane) {
        fov = fovDeg;
        aspect = aspectRatio;
        near = nearPlane;
        far = farPlane;
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    glm::mat4 Camera::get_view_matrix() {
        OPTICK_EVENT();
        glm::vec3 position = get_parent()->get_component<Component::Transform>()->position;
        glm::mat4 rotation = glm::toMat4(glm::conjugate(orientation));
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), -position);

        return rotation * translation;
    }

    glm::mat4 Camera::get_projection_matrix() {
        OPTICK_EVENT();
        return projection;
    }

    glm::quat Camera::get_orientation() {
        return orientation;
    }

    void Camera::offset_angles(float up, float right) {
        upAngle += up;
        rightAngle += right;
    }

    void Camera::update_vectors() {
        OPTICK_EVENT();
        if(upAngle > 90.0f) upAngle = 90.0f;
        if(upAngle < -90.0f) upAngle = -90.0f;
        glm::quat aroundY = glm::angleAxis(glm::radians(-rightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::quat aroundX = glm::angleAxis(glm::radians(-upAngle), glm::vec3(1.0f, 0.0f, 0.0f));
        orientation = aroundY * aroundX;
    }
}