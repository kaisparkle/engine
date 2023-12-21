#include <optick.h>
#include <glm/gtx/transform.hpp>
#include <SDL.h>
#include <player/playermanager.h>
#include <components/camera.h>

namespace Component {
    Camera::Camera(Entity::Entity *entity) : IComponent(entity) {
        update_projection(90.0f, (float)1600/(float)900, 0.1f, 10000.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        front = glm::vec3(0.0f, 0.0f, -1.0f);
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
        glm::vec3 position;
        position.x = get_parent()->get_component<Component::Transform>()->position[0];
        position.y = get_parent()->get_component<Component::Transform>()->position[1];
        position.z = get_parent()->get_component<Component::Transform>()->position[2];
        return glm::lookAt(position, position + front, up);
    }

    glm::mat4 Camera::get_projection_matrix() {
        OPTICK_EVENT();
        return projection;
    }

    void Camera::update_vectors(glm::vec3 newUp, glm::vec3 newFront) {
        OPTICK_EVENT();
        up = newUp;
        front = newFront;
    }
}