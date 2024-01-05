#include <optick.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <player/playermanager.h>
#include <component/camera.h>

namespace Component {
    Camera::Camera(Entity::Entity *entity) : IComponent(entity) {
        update_projection(90.0f, (float)1600/(float)900, 0.1f, 10000.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = up;
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        forward = front;
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
        position = glm::vec3(position.x, position.y + 2.5f, position.z);
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::get_projection_matrix() {
        OPTICK_EVENT();
        return projection;
    }

    void Camera::update_vectors(float pitch, float yaw) {
        OPTICK_EVENT();
        // calculate front vector, including the camera's pitch
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);

        // calculate forward vector, excluding the camera's pitch
        glm::vec3 newForward;
        newForward.x = cos(glm::radians(yaw));
        newForward.y = 0.0f;
        newForward.z = sin(glm::radians(yaw));
        forward = glm::normalize(newForward);

        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    glm::vec3 Camera::get_front() {
        return front;
    }

    glm::vec3 Camera::get_forward() {
        return forward;
    }

    glm::vec3 Camera::get_right() {
        return right;
    }

    glm::vec3 Camera::get_up() {
        return up;
    }
}